#pragma once
#include <atomic>
#include <iomanip>
#include <thread>
#include <asio/executor_work_guard.hpp>
#include "CHandlerSession.h"

/**
 * @brief Creates Server.
 */
class CServer
{
private:
	/**
	 * @brief .
	 */
	string m_file_name;
	/**
	 * @brief Variable to create context from asio.
	 */
	asio::io_context m_context;
	/**
	 * @brief acceptor from tcp.
	 */
	tcp::acceptor m_acceptor;
	/**
	 * @brief vector of threads to store thread clients.
	 */
	vector<thread> m_client_threads;
	/**
	 * @brief .
	 */
	atomic<chrono::steady_clock::time_point> m_last_activity;
	/**
	 * @brief flag for monitoring request activity.
	 */
	bool m_stop_requested = false;
	/**
	 * @brief work context to avoid that context stop it.
	 */
	asio::executor_work_guard<asio::io_context::executor_type> m_work_context;
	/**
	 * @brief vector of threads that will execute context run.
	 */
	vector<thread> m_threads_context;

public:
	/**
	 * @brief Constructor.
	 */
	CServer();
	/**
	 * @brief Destructor.
	 */
	~CServer();

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

