#include "CServer.h"

int main() {
    CServer server;
    // La ejecuci�n del servidor ahora es completamente aut�noma una vez iniciada.
    // Esperamos que los hilos de monitoreo terminen para que el proceso principal no termine prematuramente.
    this_thread::sleep_for(chrono::seconds(10)); // Simb�lico; en la pr�ctica, el servidor decidir� cu�ndo terminar.

    // Si se desea esperar expl�citamente a que el servidor termine (por ejemplo, basado en se�ales externas o entrada del usuario),
    // se deber�an implementar mecanismos de sincronizaci�n adicionales aqu�.

    return 0;
}