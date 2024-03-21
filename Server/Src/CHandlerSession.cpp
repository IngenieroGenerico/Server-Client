#include "CHandlerSession.h"

CHandlerSession::CHandlerSession(tcp::socket socket, int id, string* pfile_name) : m_socket(move(socket)), m_id(id)
{
	p_file_name = pfile_name;
}

CHandlerSession::~CHandlerSession()
{
}

void CHandlerSession::Start()
{
	// Asignar un nombre único al cliente
	m_client_name = "Client_" + to_string(m_id);
	cout << "New connection: " << m_client_name << endl;

	// Comenzar a manejar las interacciones con el cliente
	HandleClient();
}

void CHandlerSession::HandleClient()
{
	try
	{
		for (;;)
		{
			char data[1024];
			asio::error_code error;
			size_t length = m_socket.read_some(asio::buffer(data), error);
			m_last_request = chrono::steady_clock::now(); // Actualizar el tiempo de la última solicitud

			if (error == asio::error::eof)
			{
				cout << "Lost connection: " << m_client_name << endl;
				break; // La conexión se cerró correctamente por el cliente.
			}
			else if (error)
			{
				throw asio::system_error(error); // Otro error.
			}
			string message(data, length);

			// Procesar la solicitud del cliente
			string response = ProcessRequest(message);

			// Enviar respuesta al cliente
			asio::write(m_socket, asio::buffer(response));
		}
	}
	catch (exception& e)
	{
		cerr << "Exception HandleClient: " << m_client_name << ": " << e.what() << endl;
	}
}

chrono::steady_clock::time_point CHandlerSession::getLastRequestTime() const
{
	return m_last_request;
}


string CHandlerSession::ProcessRequest(const string& request)
{
	// Analizar la solicitud del cliente
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
		// Almacenar el usuario en el mapa
		m_id_users[id] = name;
		// Construir y devolver la respuesta
		result = "SUCCESS user added";
		// Escribir en el archivo
		std::ofstream outFile(*p_file_name, std::ios::app); // Usar el nombre de archivo del servidor
		if (outFile.is_open())
		{
			outFile << "Client ID: " << this->m_id << ", User: " << name << " (ID: " << id << ")" << std::endl;
		}
		else
		{
			cerr << "Error when open file to write." << endl;
		}
	}
	cout << result << " request from: " << m_client_name << endl;
	return result;
}
