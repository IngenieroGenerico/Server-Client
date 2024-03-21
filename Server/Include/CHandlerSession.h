#pragma once
#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <asio.hpp>
#include <asio/ip/tcp.hpp>
#include <map>
#include "ConsoleStyle.h"


using namespace std;
using asio::ip::tcp;
static std::string fileName; // Nombre de archivo basado en la fecha y 
class CHandlerSession {

public:
    CHandlerSession(tcp::socket socket, int id);
    void start();
    void handleClient();

    chrono::steady_clock::time_point getLastRequestTime()const;

private:
    tcp::socket socket;
    int id;
    string clientName;
    map<int, string> userMap; // Almacenar usuarios por su ID
    chrono::steady_clock::time_point lastRequestTime; // Tiempo de la última solicitud

    string processRequest(const string& request);
};

