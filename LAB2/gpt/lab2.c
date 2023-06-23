#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    // Validar argumentos de línea de comando
    int option;
    char* archivoEntrada = NULL;
    char* archivoSalida = NULL;
    int workers = 0;
    int chunkSize = 0;
    int b = 0;
    while ((option = getopt(argc, argv, "i:o:w:c:b")) != -1) {
        switch (option) {
            case 'i':
                archivoEntrada = optarg;
                break;
            case 'o':
                archivoSalida = optarg;
                break;
            case 'w':
                workers = atoi(optarg);
                break;
            case 'c':
                chunkSize = atoi(optarg);
                break;
            case 'b':
                b = 1;
                break;
        }
    }

    if (archivoEntrada == NULL) {
        printf("Error: falta nombre de archivo de entrada\n");
        return 1;
    }
    if (archivoSalida == NULL) {
        printf("Error: falta nombre de archivo de salida\n");
        return 1;
    }
    if (workers == 0) {
        printf("Error: falta cantidad de workers\n");
        return 1;
    }
    if (chunkSize == 0) {
        printf("Error: falta tamaño de chunk\n");
        return 1;
    }

    // Ejecutar proceso broker
    int pid = fork();

    if (pid < 0) {
        printf("Error al crear proceso broker.\n");
        return 1;
    } else if (pid == 0) {
        // Proceso hijo (broker)
        char str_workers[10];
        char str_chunkSize[10];
        sprintf(str_workers, "%d", workers);
        sprintf(str_chunkSize, "%d", chunkSize);
        execl("./broker", "broker", archivoEntrada, archivoSalida, str_workers,
              str_chunkSize, NULL);
        printf("Error al ejecutar proceso broker. 2\n");
        return 1;
    } else {
        // Proceso padre
        wait(NULL);  // Esperar a que el proceso hijo termine
        printf("Proceso padre finalizado.\n");
    }

    return 0;
}
