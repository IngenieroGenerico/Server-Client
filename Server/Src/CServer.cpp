#include "CServer.h"
CServer::CServer() : acceptor(io_context, tcp::endpoint(tcp::v4(), 8080)),
lastActivityTime(chrono::steady_clock::now()), work(asio::make_work_guard(io_context)) 
{


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


void CServer::generateFile() 
{
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

void CServer::startAccept() 
{
    while (!stopRequested) 
    {
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
            CHandlerSession* newSession = new CHandlerSession(move(socket), this->clientThreads.size());
            newSession->start();
            this->lastActivityTime.store(chrono::steady_clock::now()); // Actualizar la última actividad
            });
    }
}

void CServer::activityMonitor() 
{
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

CServer::~CServer() 
{
    // Asegurarse de que todos los hilos se unan antes de finalizar
    for (auto& t : clientThreads) 
    {
        if (t.joinable()) 
        {
            t.join();
        }
    }
    for (auto& t : ioContextThreads) // Asegurar que los hilos de io_context.run() también se unan
    { 
        if (t.joinable()) 
        {
            t.join();
        }
    }
}