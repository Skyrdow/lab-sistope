#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define SMALLBUFFERMAX 16

int main(int argc, char* argv[]) {
    // Validar argumentos de línea de comando
    int option;
    char* archivoEntrada = NULL;
    char* archivoSalida = NULL;
    int workers = 0;
    int chunkSize = 0;
    int b = 0;
    while ((option = getopt(argc, argv, "i:o:n:c:b")) != -1) {
        switch (option) {
            case 'i':
                archivoEntrada = optarg;
                break;
            case 'o':
                archivoSalida = optarg;
                break;
            case 'n':
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
    if (workers <= 0) {
        printf("Error: falta cantidad de workers\n");
        return 1;
    }
    if (chunkSize <= 0) {
        printf("Error: falta tamaño de chunk\n");
        return 1;
    }

    if (b != 0 && b != 1) {
        printf("Error: valor de b incorrecto\n");
        return 1;
    }

    // Ejecutar proceso broker
    int pid = fork();
    int fd[2];
    if (pipe(fd) == -1)
    {
        printf("Error al crear el pipe\n");
        exit(1);
    }
    dup2(STDOUT_FILENO, fd[1]);

    if (pid < 0) {
        printf("Error al crear proceso broker.\n");
        return 1;
    } else if (pid == 0) {
        // Proceso hijo (broker)
        dup2( fd[1], STDOUT_FILENO);
        char str_workers[SMALLBUFFERMAX];
        char str_chunkSize[SMALLBUFFERMAX];
        char str_fd[SMALLBUFFERMAX];
        char str_b[SMALLBUFFERMAX];
        sprintf(str_workers, "%d", workers);
        sprintf(str_chunkSize, "%d", chunkSize);
        sprintf(str_fd, "%d", fd[1]);
        sprintf(str_b, "%d", b);
        char* args[] = {"broker", archivoEntrada, archivoSalida,
                        str_workers, str_chunkSize, str_fd, str_b, NULL};
        execv("./broker", args);
        printf("Error al ejecutar proceso broker.\n");
        return 1;
    } else {
        // Proceso padre
        wait(NULL);  // Esperar a que el proceso hijo termine
        // printf("Proceso padre finalizado.\n");
    }

    return 0;
}
