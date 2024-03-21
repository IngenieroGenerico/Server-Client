#pragma once
///******************************************************/
///*			C++ Standar Library & STL.				*/
///******************************************************/
#include <iostream>
#include <string>
#include <thread>
#include <chrono>
///******************************************************/
///*					Asio.							*/
///******************************************************/
#include <asio.hpp>
#include <asio/ip/tcp.hpp>
///******************************************************/
///*				Proyect Headers.					*/
///******************************************************/
#include "ConsoleStyle.h"
#include "CUser.h"

// using namespace of asio::ip::tcp to avoid to write it.
using asio::ip::tcp;

/**
 * @brief Use it to create a client.
 */
class CClient
{
	///**************************************************************************/
	///*						  Member Variables.								*/
	///**************************************************************************/
	
private:
	/**
	 * @brief Constant that define the max ids amount it will be shared throught all class instances.
	 */
	static const int MAX_IDS = 252;
	/**
	 * @brief vector where all ids used will be stored.
	 */
	vector<int> m_ids;

	///************************************************************************/
	///*                            Constructor & Destructor.                 */
	///************************************************************************/
	
public:
	/**
	 * @brief Constructor
	 */
	CClient();
	/**
	 * @brief Destructor.
	 */
	~CClient();
	
	///************************************************************************/
	///*						   Class Functions.							  */
	///************************************************************************/
	
public:

	/**
	 * @brief Function to initialize the client.
	 */
	void Start();

private:
	
	/**
	 * @brief Save id into class vector
	 * @param int: id from actual user.
	 */
	void StoreId(int id);
};

