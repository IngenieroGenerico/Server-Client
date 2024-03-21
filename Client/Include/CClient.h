#pragma once
#include <iostream>
#include <string>
#include <asio.hpp>
#include <asio/ip/tcp.hpp>
#include <chrono>
#include <thread>
#include "ConsoleStyle.h"
#include "CUser.h"

using asio::ip::tcp;

/**
 * @brief Use it to create a client.
 */
class CClient
{
private:
	static const int MAX_IDS = 252;
	vector<int> m_ids;
public:
	/**
	 * @brief Constructor
	 */
	CClient();
	/**
	 * @brief Destructor.
	 */
	~CClient();
	/**
	 * @brief Function to initialize the client. 
	 */
	void Start();

private:
	
	void StoreId(int id);
	void HandleResponse();

};

