#include "CServer.h"

int main() {
    CServer server;
    // La ejecución del servidor ahora es completamente autónoma una vez iniciada.
    // Esperamos que los hilos de monitoreo terminen para que el proceso principal no termine prematuramente.
    this_thread::sleep_for(chrono::seconds(10)); // Simbólico; en la práctica, el servidor decidirá cuándo terminar.

    // Si se desea esperar explícitamente a que el servidor termine (por ejemplo, basado en señales externas o entrada del usuario),
    // se deberían implementar mecanismos de sincronización adicionales aquí.

    return 0;
}