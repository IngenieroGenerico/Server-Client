#include "CServer.h"

// Initialze variables in the constructor.
CServer::CServer() : m_acceptor(m_context, tcp::endpoint(tcp::v4(), 8080)),
m_last_activity(chrono::steady_clock::now()), m_work_context(asio::make_work_guard(m_context))
{
	//Create output file.
	CreateOutputFile();
	//Initialize threads for listening connections, activity and context.
	StartThreads();
}

void CServer::StartThreads()
{
	//Start conections aceptations and activity monitor on separated threads.
	thread([this]() { this->Start(); }).detach();
	thread([this]() { this->ActivityMonitor(); }).detach();
	// Run threads to execute io_context.run()
	// Use double of the threads available from the hardware.
	const auto numThreads = max(2u, thread::hardware_concurrency() * 2); 
	for (unsigned i = 0; i < numThreads; ++i)
	{
		m_threads_context.emplace_back([this] { m_context.run(); });
	}
}

void CServer::CreateOutputFile()
{
	// Get actual time.
	std::time_t now = std::time(nullptr);
	std::tm timeStruct = {}; 

	//Fill timestruct with local time.
	localtime_s(&timeStruct, &now);

	//Convert time to string with the specific format.
	std::ostringstream ss;
	ss << std::put_time(&timeStruct, "users_%Y-%m-%d_%Hh-%Mm-%Ss.txt"); 
	m_file_name = ss.str();
}

//Initialize Server.
void CServer::Start()
{
	while (!m_stop_requested)
	{
		tcp::socket socket(m_context);
		// El método accept() puede lanzar una excepción si el acceptor es cerrado, por lo que es necesario manejarlo adecuadamente.
		try
		{
			m_acceptor.accept(socket);
		}
		catch (exception& e)
		{
			if (m_stop_requested) break; // Salir del bucle si se solicitó detener el servidor
			throw; // Volver a lanzar la excepción si fue por otra razón
		}

		cout << "New request connection." << endl;
		m_client_threads.emplace_back([this, socket = move(socket)]() mutable {
			CHandlerSession* newSession = new CHandlerSession(move(socket), this->m_client_threads.size(), &m_file_name);
			newSession->Start();
			this->m_last_activity.store(chrono::steady_clock::now()); // Actualizar la última actividad
			});
	}
}

void CServer::ActivityMonitor()
{
	//Check if there is not request for stop the server.
	while (!m_stop_requested)
	{
		//Get actual time and last activity.
		auto currentTime = chrono::steady_clock::now();
		auto lastActivity = m_last_activity.load();
		//Check if the last time that an activity was made it is more than 1 minute.
		if (chrono::duration_cast<chrono::minutes>(currentTime - lastActivity) >= chrono::minutes(1))
		{
			cout << "Not request received for more than 1 minute..Shutting down the server." << endl;
			m_stop_requested = true;
			m_work_context.reset(); //Reset context if there is no more work to do.
			m_acceptor.close(); //Close acceptor to finish accept loop.
			m_context.stop(); //Stop io_context to finish any pendent asyncrhonous operation.

			// Wait 5 seconds before stop the Server to avoid abrupt shutdown.
			std::this_thread::sleep_for(std::chrono::seconds(5));
			break;
		}
		//Wait before next comparation for activity.
		this_thread::sleep_for(chrono::seconds(10)); 
	}
}

CServer::~CServer()
{
	//Make sure all threads join together at the end to avoid conflicts.
	for (auto& t : m_client_threads)
	{
		if (t.joinable())
		{
			t.join();
		}
	}
	//Make sure all threads for context join together to.
	for (auto& t : m_threads_context)
	{
		if (t.joinable())
		{
			t.join();
		}
	}
}