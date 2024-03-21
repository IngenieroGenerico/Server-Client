#pragma once
#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <map>
#include <asio.hpp>
#include <asio/ip/tcp.hpp>
#include "ConsoleStyle.h"

using namespace std;
using asio::ip::tcp;

/**
 * @brief .
 */
class CHandlerSession
{

public:
	/**
	 * @brief Constructor.
	 * @param socket
	 * @param id
	 */
	CHandlerSession(tcp::socket socket, int id, string* pfile_name);
	/**
	 * @brief Destructor.
	 */
	~CHandlerSession();
	/**
	 * @brief .
	 */
	void Start();
	/**
	 * @brief .
	 */
	void HandleClient();
	/**
	 * @brief .
	 * @return
	 */
	chrono::steady_clock::time_point getLastRequestTime()const;

private:
	/**
	 * @brief Pointer to file name where all final data will be stored.
	 */
	string* p_file_name;
	/**
	 * @brief .
	 */
	tcp::socket m_socket;
	/**
	 * @brief .
	 */
	int m_id;
	/**
	 * @brief .
	 */
	string m_client_name;
	/**
	 * @brief .
	 */
	map<int, string> m_id_users; // Almacenar usuarios por su ID
	/**
	 * @brief .
	 */
	chrono::steady_clock::time_point m_last_request; // Tiempo de la última solicitud
	/**
	 * @brief .
	 * @param request
	 * @return 
	 */
	string ProcessRequest(const string& request);
};

