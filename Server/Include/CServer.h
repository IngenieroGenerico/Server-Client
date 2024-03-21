#pragma once
///******************************************************/
///*			C++ Standar Library & STL.				*/
///******************************************************/
#include <atomic>
#include <iomanip>
#include <thread>
#include <memory>
///******************************************************/
///*					Asio.							*/
///******************************************************/
#include <asio/executor_work_guard.hpp>
///******************************************************/
///*				Proyect Headers.					*/
///******************************************************/
#include "CHandlerSession.h"


/**
 * @brief Creates Server.
 */
class CServer
{

	///**************************************************************************/
	///*						  Member Variables.								*/
	///**************************************************************************/

private:
	
	/**
	 * @brief flag for monitoring request activity.
	 */
	bool m_stop_requested = false;
	/**
	 * @brief save the file name.
	 */
	string m_file_name;
	/**
	 * @brief Variable to create context from asio.
	 */
	asio::io_context m_context;
	/**
	 * @brief work context to avoid that context stop it.
	 */
	asio::executor_work_guard<asio::io_context::executor_type> m_work_context;
	/**
	 * @brief acceptor from tcp.
	 */
	tcp::acceptor m_acceptor;
	/**
	 * @brief vector of threads to store thread clients.
	 */
	vector<thread> m_client_threads;
	/**
	 * @brief vector of threads that will execute context run.
	 */
	vector<thread> m_threads_context;
	/**
	 * @brief .
	 */
	atomic<chrono::steady_clock::time_point> m_last_activity;

	///************************************************************************/
	///*                            Constructor & Destructor.                 */
	///************************************************************************/

public:

	/**
	 * @brief Constructor.
	 */
	CServer();
	/**
	 * @brief Destructor.
	 */
	~CServer();

	///************************************************************************/
	///*						   Class Functions.							  */
	///************************************************************************/
	
public:

	/**
	 * @brief Creates output file clients' insertions.
	 */
	void CreateOutputFile();
	/**
	 * @brief Initialize server and contexts.
	 */
	void Start();
	/**
	 * @brief Initialize threads.
	 */
	void StartThreads();
	/**
	 * @brief Check requests activity for clients.
	 * 
	 */
	void ActivityMonitor();
	
};

