#include "CHandlerSession.h"


CHandlerSession::CHandlerSession(tcp::socket socket, int id) : socket(move(socket)), id(id)
{
}

void CHandlerSession::start()
{
	// Asignar un nombre único al cliente
	clientName = "Client_" + to_string(id);
	cout << "New connection: " << clientName << endl;

	// Comenzar a manejar las interacciones con el cliente
	handleClient();
}

void CHandlerSession::handleClient()
{
	try
	{
		for (;;)
		{
			char data[1024];
			asio::error_code error;
			size_t length = socket.read_some(asio::buffer(data), error);
			lastRequestTime = chrono::steady_clock::now(); // Actualizar el tiempo de la última solicitud

			if (error == asio::error::eof)
			{
				cout << "Lost connection: " << clientName << endl;
				break; // La conexión se cerró correctamente por el cliente.
			}
			else if (error)
			{
				throw asio::system_error(error); // Otro error.
			}
			string message(data, length);

			// Procesar la solicitud del cliente
			string response = processRequest(message);

			// Enviar respuesta al cliente
			asio::write(socket, asio::buffer(response));
		}
	}
	catch (exception& e)
	{
		cerr << "Exception HandleClient: " << clientName << ": " << e.what() << endl;
	}
}

chrono::steady_clock::time_point CHandlerSession::getLastRequestTime() const
{
	return lastRequestTime;
}


string CHandlerSession::processRequest(const string& request)
{
	// Analizar la solicitud del cliente
	stringstream ss(request);
	int id;
	string name;
	ss >> id >> name;
	string result;

	// Verificar si el ID ya está en uso
	if (userMap.find(id) != userMap.end())
	{
		SetConsoleColor(RED);
		result = "DENIED id already exist";
	}
	else
	{
		SetConsoleColor(WHITE);
		// Almacenar el usuario en el mapa
		userMap[id] = name;
		// Construir y devolver la respuesta
		result = "SUCCESS user added";
		// Escribir en el archivo
		std::ofstream outFile(fileName, std::ios::app); // Usar el nombre de archivo del servidor
		if (outFile.is_open())
		{
			outFile << "Client ID: " << this->id << ", User: " << name << " (ID: " << id << ")" << std::endl;
		}
		else
		{
			cerr << "Error when open file to write." << endl;
		}
	}
	cout << result << " request from: " << clientName << endl;
	return result;
}
