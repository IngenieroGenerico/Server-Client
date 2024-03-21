#include <atomic>
#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <asio.hpp>
#include <asio/ip/tcp.hpp>
#include <asio/executor_work_guard.hpp>
#include <map>
#include <iomanip>
#include <thread>
#include "ConsoleStyle.h"


using namespace std;
using asio::ip::tcp;

static std::string fileName; // Nombre de archivo basado en la fecha y 

class ClientSession {

public:
    ClientSession(tcp::socket socket, int id) : socket(move(socket)), id(id) {}

    void start() {
        // Asignar un nombre único al cliente
        clientName = "Client_" + to_string(id);
        cout << "New connection: " << clientName << endl;

        // Comenzar a manejar las interacciones con el cliente
        handleClient();
    }

    void handleClient() {
        try {
            for (;;) {
                char data[1024];
                asio::error_code error;
                size_t length = socket.read_some(asio::buffer(data), error);
                lastRequestTime = chrono::steady_clock::now(); // Actualizar el tiempo de la última solicitud

                if (error == asio::error::eof) {
                    cout << "Lost connection: " << clientName << endl;
                    break; // La conexión se cerró correctamente por el cliente.
                }
                else if (error) {
                    throw asio::system_error(error); // Otro error.
                }
                string message(data, length);

                // Procesar la solicitud del cliente
                string response = processRequest(message);

                // Enviar respuesta al cliente
                asio::write(socket, asio::buffer(response));
            }
        }
        catch (exception& e) {
            cerr << "Exception HandleClient: " << clientName << ": " << e.what() << endl;
        }
    }

    chrono::steady_clock::time_point getLastRequestTime() const {
        return lastRequestTime;
    }

private:
    tcp::socket socket;
    int id;
    string clientName;
    map<int, string> userMap; // Almacenar usuarios por su ID
    chrono::steady_clock::time_point lastRequestTime; // Tiempo de la última solicitud

    string processRequest(const string& request) {
        // Analizar la solicitud del cliente
        stringstream ss(request);
        int id;
        string name;
        ss >> id >> name;
        string result;

        // Verificar si el ID ya está en uso
        if (userMap.find(id) != userMap.end()) {
            SetConsoleColor(RED);
            result = "DENIED id already exist";
        }
        else {
            SetConsoleColor(WHITE);
            // Almacenar el usuario en el mapa
            userMap[id] = name;
            // Construir y devolver la respuesta
            result = "SUCCESS user added";
            // Escribir en el archivo
            std::ofstream outFile(fileName, std::ios::app); // Usar el nombre de archivo del servidor
            if (outFile.is_open()) {
                outFile << "Client ID: " << this->id << ", User: " << name << " (ID: " << id << ")" << std::endl;
            }
            else {
                cerr << "Error when open file to write." << endl;
            }
        }
        cout << result << " request from: " << clientName << endl;
        return result;
    }
};



class Server {
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
    Server() : acceptor(io_context, tcp::endpoint(tcp::v4(), 8080)), 
        lastActivityTime(chrono::steady_clock::now()), work(asio::make_work_guard(io_context)) {

        
        generateFile();
        // Iniciar la aceptación de conexiones y el monitoreo de actividad en hilos separados
        thread([this]() { this->startAccept(); }).detach();
        thread([this]() { this->activityMonitor(); }).detach();
        // Lanzar hilos para ejecutar io_context.run()
        const auto numThreads = max(2u, thread::hardware_concurrency() * 2); // Ejemplo: usar el doble de núcleos disponibles
        for (unsigned i = 0; i < numThreads; ++i) {
            ioContextThreads.emplace_back([this] { io_context.run(); });
        }
    }

    
    void generateFile() {
        // Obtener el tiempo actual
        std::time_t now = std::time(nullptr);
        std::tm timeStruct = {}; // Crear una estructura tm vacía.

        // Uso seguro dependiendo del entorno (en este caso, para MSVC)
        localtime_s(&timeStruct, &now); // Rellenar timeStruct con el tiempo local

        // Convertir tiempo a string con formato específico
        std::ostringstream ss;
        ss << std::put_time(&timeStruct, "users_%Y-%m-%d_%Hh-%Mm-%Ss.txt"); // Formato: users_AAAAmmdd_HHMMSS.txt
        fileName = ss.str();
    }

    void startAccept() {
        while (!stopRequested) {
            tcp::socket socket(io_context);
            // El método accept() puede lanzar una excepción si el acceptor es cerrado, por lo que es necesario manejarlo adecuadamente.
            try {
                acceptor.accept(socket);
            }
            catch (exception& e) {
                if (stopRequested) break; // Salir del bucle si se solicitó detener el servidor
                throw; // Volver a lanzar la excepción si fue por otra razón
            }

            cout << "New request connection." << endl;
            clientThreads.emplace_back([this, socket = move(socket)]() mutable {
                ClientSession* newSession = new ClientSession(move(socket), this->clientThreads.size());
                newSession->start();
                this->lastActivityTime.store(chrono::steady_clock::now()); // Actualizar la última actividad
                });
        }
    }

    void activityMonitor() {
        while (!stopRequested) {
            auto currentTime = chrono::steady_clock::now();
            auto lastActivity = lastActivityTime.load();
            if (chrono::duration_cast<chrono::minutes>(currentTime - lastActivity) >= chrono::minutes(1)) {
                cout << "Not request received for more than 1 minute..Shutting down the server." << endl;
                stopRequested = true;
                work.reset(); // Permitir que io_context finalice si no hay más trabajo
                acceptor.close(); // Cerrar el acceptor para terminar el bucle de accept.
                io_context.stop(); // Detener el io_context para finalizar cualquier operación asincrónica pendiente.

                // Espera de 5 segundos antes de finalizar completamente
                std::this_thread::sleep_for(std::chrono::seconds(5));
                break;
            }
            this_thread::sleep_for(chrono::seconds(10)); // Esperar antes de la siguiente comprobación
        }
    }

    ~Server() {
        // Asegurarse de que todos los hilos se unan antes de finalizar
        for (auto& t : clientThreads) {
            if (t.joinable()) {
                t.join();
            }
        }
        for (auto& t : ioContextThreads) { // Asegurar que los hilos de io_context.run() también se unan
            if (t.joinable()) {
                t.join();
            }
        }
    }
};

int main() {
    Server server;
    // La ejecución del servidor ahora es completamente autónoma una vez iniciada.
    // Esperamos que los hilos de monitoreo terminen para que el proceso principal no termine prematuramente.
    this_thread::sleep_for(chrono::seconds(10)); // Simbólico; en la práctica, el servidor decidirá cuándo terminar.

    // Si se desea esperar explícitamente a que el servidor termine (por ejemplo, basado en señales externas o entrada del usuario),
    // se deberían implementar mecanismos de sincronización adicionales aquí.

    return 0;
}