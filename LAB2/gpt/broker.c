#include "fbroker.h"

int main(int argc, char* argv[]) {
    // Validar argumentos de línea de comando
    if (argc != 5) {
        printf(
            "Uso: %s <archivo_entrada> <archivo_salida> <cantidad_workers> "
            "<numero_chunks>\n",
            argv[0]);
        return 1;
    }

    char* archivoEntrada = argv[1];
    char* archivoSalida = argv[2];
    int workers = atoi(argv[3]);
    int chunkSize = atoi(argv[4]);

    // Procesar archivo
    procesarArchivo(archivoEntrada, archivoSalida, workers,
                    chunkSize);

    return 0;
}
