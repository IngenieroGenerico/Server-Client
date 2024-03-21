#include "CHandlerSession.h"

//Constructor
CHandlerSession::CHandlerSession(tcp::socket socket, int id, string* pfile_name) : m_socket(move(socket)), m_id(id)
{
	p_file_name = pfile_name;
}

//Destructor
CHandlerSession::~CHandlerSession()
{
}

//Initialize HandlerSession process.
void CHandlerSession::Start()
{
	//Assign a unique name from this client.
	m_client_name = "Client_" + to_string(m_id);
	cout << "New connection: " << m_client_name << endl << endl;
	//Start handle client interactions.
	HandleClient();
}

//Handler clients comming from Client proyect.
void CHandlerSession::HandleClient()
{
	try
	{
		for (;;)
		{
			char data[1024];
			asio::error_code error;
			size_t length = m_socket.read_some(asio::buffer(data), error);

			if (error == asio::error::eof)
			{
				//Connection was closed correctly.
				cout << "Lost connection: " << m_client_name << endl;
				break;
			}
			else if (error)
			{
				//throw unexpected error.
				throw asio::system_error(error); 
			}

			string message(data, length);
			//Process client request.
			string response = ProcessRequest(message);
			//Send response to client.
			asio::write(m_socket, asio::buffer(response));
		}
	}
	catch (exception& e)
	{
		cerr << "Exception HandleClient: " << m_client_name << ": " << e.what() << endl;
	}
}

// Function where request will be process.
string CHandlerSession::ProcessRequest(const string& request)
{
	// Get the request and transform into its respectible variables.
	stringstream ss(request);
	int id;
	string name;
	ss >> id >> name;
	string result;

	// Verificar si el ID ya está en uso
	if (m_id_users.find(id) != m_id_users.end())
	{
		SetConsoleColor(RED);
		result = "DENIED id already exist";
	}
	else
	{
		SetConsoleColor(WHITE);
		// Store user id and name.
		m_id_users[id] = name;
		// Construct response to send it to the client.
		result = "SUCCESS user added";
		WriteInOutputFile(id, name);
	}
	cout << result << " request from: " << m_client_name << endl;
	return result;
}

// Write user info comming for the client into the Output file.
void CHandlerSession::WriteInOutputFile(int id, string name)
{
	// Write into the file comming from Server throught the pointer.
	std::ofstream outFile(*p_file_name, std::ios::app); 
	if (outFile.is_open())
	{
		outFile << "Client ID: " << this->m_id << ", User: " << name << " (ID: " << id << ")" << std::endl;
	}
	else
	{
		cerr << "Error when open file to write." << endl;
	}
}
