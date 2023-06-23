#include "fbroker.h"

void procesarArchivo(const char *archivoEntrada, const char *archivoSalida, int cantidadWorkers, int numeroChunks) {
    // Crear pipes para comunicación con los workers
    int pipes[cantidadWorkers][2];
    for (int i = 0; i < cantidadWorkers; i++) {
        if (pipe(pipes[i]) == -1) {
            printf("Error al crear pipe.\n");
            exit(1);
        }
    }

    // Ejecutar los workers
    int pids[cantidadWorkers];
    for (int i = 0; i < cantidadWorkers; i++) {
        int pid = fork();
        if (pid < 0) {
            printf("Error al crear proceso worker.\n");
            exit(1);
        } else if (pid == 0) {
            // Proceso hijo (worker)
            close(pipes[i][0]);  // Cerrar extremo de lectura del pipe
            dup2(pipes[i][1], STDOUT_FILENO);  // Redirigir salida estándar al pipe
            execl("worker", "worker", NULL);
            printf("Error al ejecutar proceso worker.\n");
            exit(1);
        } else {
            // Proceso padre
            pids[i] = pid;
            close(pipes[i][1]);  // Cerrar extremo de escritura del pipe
        }
    }

    // Abrir archivo de entrada
    FILE *archivo = fopen(archivoEntrada, "r");
    if (archivo == NULL) {
        printf("Error al abrir el archivo de entrada.\n");
        exit(1);
    }

    // Leer líneas del archivo y enviarlas a los workers
    char linea[256];
    int workerActual = 0;
    int lineasProcesadas[cantidadWorkers];
    memset(lineasProcesadas, 0, sizeof(lineasProcesadas));

    while (fgets(linea, sizeof(linea), archivo) != NULL) {
        if (write(pipes[workerActual][1], linea, strlen(linea)) == -1) {
            printf("Error al escribir en el pipe.\n");
            exit(1);
        }

        lineasProcesadas[workerActual]++;
        workerActual = (workerActual + 1) % cantidadWorkers;
    }

    // Enviar señal de FIN a los workers
    for (int i = 0; i < cantidadWorkers; i++) {
        if (write(pipes[i][1], "FIN", 3) == -1) {
            printf("Error al escribir en el pipe.\n");
            exit(1);
        }
    }

    // Esperar a que los workers terminen y obtener la cantidad de líneas procesadas
    int estado;
    for (int i = 0; i < cantidadWorkers; i++) {
        waitpid(pids[i], &estado, 0);
        printf("Worker %d procesó %d líneas.\n", pids[i], lineasProcesadas[i]);
    }

    // Cerrar archivo de entrada
    fclose(archivo);

    // Realizar otras tareas necesarias, como escribir los resultados en el archivo de salida
    // y mostrarlos en la salida estándar si se utiliza la bandera -b

    printf("Proceso broker finalizado.\n");
}
