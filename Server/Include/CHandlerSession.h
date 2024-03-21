#pragma once
///******************************************************/
///*			C++ Standar Library & STL.				*/
///******************************************************/
#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <map>
///******************************************************/
///*					Asio.							*/
///******************************************************/
#include <asio.hpp>
#include <asio/ip/tcp.hpp>
///******************************************************/
///*				Proyect Headers.					*/
///******************************************************/
#include "ConsoleStyle.h"

// using namespace of std and asio::ip::tcp to avoid to write it.
using namespace std;
using asio::ip::tcp;

/**
 * @brief Class for handler all sessions comming for client application.
 */
class CHandlerSession
{
	///**************************************************************************/
	///*						  Member Variables.								*/
	///**************************************************************************/
	
private:
	/**
	 * @brief Pointer to file name where all final data will be stored.
	 */
	string* p_file_name;
	/**
	 * @brief Used to create communications.
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
	 * @brief Save users and ids comming from request.
	 */
	map<int, string> m_id_users; 

	///************************************************************************/
	///*                            Constructor & Destructor.                 */
	///************************************************************************/

public:
	/**
	 * @brief Constructor.
	 * @param tcp::socket: socket for create communication.
	 * @param int: id for client.
	 * @param string*: pointer to file name from server.
	 */
	CHandlerSession(tcp::socket socket, int id, string* pfile_name);
	/**
	 * @brief Destructor.
	 */
	~CHandlerSession();


	///************************************************************************/
	///*						   Class Functions.							  */
	///************************************************************************/

public:
	/**
	 * @brief Initialize HandlerSession process.
	 */
	void Start();
	/**
	 * @brief Handler clients comming from Client proyect.
	 */
	void HandleClient();
	/**
	 * @brief Function where request will be process.
	 * @param const string&: constant reference of the request.
	 * @return 
	 */
	string ProcessRequest(const string& request);

private:

	/**
	 * @brief Write user info comming for the client into the Output file.
	 * @param int: id of the actual user.
	 * @param string: name of the actual user.
	 */
	void WriteInOutputFile(int id, string name);
};

