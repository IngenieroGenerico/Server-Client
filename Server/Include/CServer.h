#pragma once
#include <atomic>
#include <asio/executor_work_guard.hpp>
#include <iomanip>
#include <thread>
#include "CHandlerSession.h"

class CServer
{
    private:

        asio::io_context io_context;
        tcp::acceptor acceptor;
        vector<thread> clientThreads;
        atomic<chrono::steady_clock::time_point> lastActivityTime;
        bool stopRequested = false; // Flag para controlar el ciclo de monitoreo de actividad
        asio::executor_work_guard<asio::io_context::executor_type> work;
        vector<thread> threads;
        vector<thread> ioContextThreads; // Almacenará los hilos que ejecutan io_context.run()


    public:
        CServer();
        


        void generateFile();
        void startAccept();

        void activityMonitor();

        ~CServer();
};

