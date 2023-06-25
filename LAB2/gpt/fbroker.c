#include "fbroker.h"

int getLineCount(const char* input_file_name) {
    int line_count = 0;
    FILE* fp = fopen(input_file_name, "r");
    char k[EXPR_MAX];
    while (fgets(k, sizeof(k), fp)) {
        if (k[0] != '\n')
            line_count++;
    }
    fclose(fp);
    return line_count;
}

void procesarArchivo(const char* archivoEntrada,
                     const char* archivoSalida,
                     int cantidadWorkers,
                     int chunkSize) {
    // pipes[i * 2] padre
    // pipes[i * 2 + 1] hijo
    int pipes[cantidadWorkers * 2][2];
    for (int i = 0; i < cantidadWorkers; i++) {
        if (pipe(pipes[i]) == -1) {
            printf("Error al crear pipe.\n");
            exit(1);
        }
    }

    int cantidadLineas = getLineCount(archivoEntrada);
    int cantidadChunks = cantidadLineas / chunkSize;
    int repeticionWorkers = cantidadChunks / cantidadWorkers;
    int restoRepeticion = cantidadChunks % cantidadWorkers;

    // Ejecutar los workers
    int pids[cantidadWorkers];
    for (int i = 0; i < cantidadWorkers; i++) {
        int pid = fork();
        if (pid < 0) {
            printf("Error al crear proceso worker.\n");
            exit(1);
        } else if (pid == 0) {
            close(pipes[i * 2][0]);  // Cerrar extremo de lectura del pipe padre
            close(pipes[i * 2 + 1]
                       [1]);  // Cerrar extremo de escritura del pipe hijo
            char fd_Pbuffer[100];
            char fd_Hbuffer[100];
            sprintf(fd_Pbuffer, "%d", pipes[i * 2][1]);
            sprintf(fd_Hbuffer, "%d", pipes[i * 2 + 1][0]);

            char* args[] = {"worker", fd_Pbuffer, fd_Hbuffer, NULL};
            execv("worker", args);
            printf("Error al ejecutar proceso worker.\n");
            exit(1);
        } else {
            // Proceso padre
            // Cerrar extremo de escritura del pipe padre
            close(pipes[i * 2][1]);
            // Cerrar extremo de lectura del pipe hijo
            close(pipes[i * 2 + 1][0]);
            pids[i] = pid;
            // close(pipes[i][1]);  // Cerrar extremo de escritura del pipe
        }
    }

    // Abrir archivo de entrada
    FILE* archivo = fopen(archivoEntrada, "r");
    if (archivo == NULL) {
        printf("Error al abrir el archivo de entrada.\n");
        exit(1);
    }

    // Leer líneas del archivo y enviarlas a los workers
    char linea[EXPR_MAX];
    int workerActual = 0;
    int lineasProcesadas[cantidadWorkers];
    memset(lineasProcesadas, 0, sizeof(lineasProcesadas));

    while (fgets(linea, sizeof(char) * 60, archivo) != NULL) {
        if (linea[0] == '\n')
            continue;
        write(pipes[2 * workerActual][1], linea, strlen(linea));

        char buffer[100];
        read(pipes[2 * workerActual + 1][0], buffer, sizeof(buffer));

        lineasProcesadas[workerActual]++;
        if (lineasProcesadas[workerActual] == chunkSize)
            workerActual = (workerActual + 1) % cantidadWorkers;

        printf("Proceso terminado %s\n", buffer);
    }

    for (int i = 0; i < cantidadWorkers; i++) {
        write(pipes[2 * i + 1][1], "FIN", 3);
        read(pipes[2 * i][0], linea, sizeof(linea));
    }
}

void procesarArchivo2(const char* archivoEntrada,
                      const char* archivoSalida,
                      int cantidadWorkers,
                      int chunkSize) {
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
            close(pipes[i][0]);  // Cerrar extremo de lectura del pipe
            execl("worker", "worker", NULL);
            printf("Error al ejecutar proceso worker.\n");
            exit(1);
        } else {
            // Proceso padre
            pids[i] = pid;
            // close(pipes[i][1]);  // Cerrar extremo de escritura del pipe
        }
    }

    // Abrir archivo de entrada
    FILE* archivo = fopen(archivoEntrada, "r");
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
        if (write(pipes[workerActual][1], linea, strlen(linea) - 1) == -1) {
            printf("Error al escribir en el pipe.\n");
            exit(1);
        }

        lineasProcesadas[workerActual]++;
        if (lineasProcesadas[workerActual] == chunkSize)
            workerActual = (workerActual + 1) % cantidadWorkers;
    }

    // Enviar señal de FIN a los workers
    for (int i = 0; i < cantidadWorkers; i++) {
        if (write(pipes[i][1], "FIN", 3) == -1) {
            printf("Error al escribir en el pipe.\n");
            exit(1);
        }
    }
    // Esperar a que los workers terminen y obtener la cantidad de líneas
    // procesadas
    int estado;
    for (int i = 0; i < cantidadWorkers; i++) {
        waitpid(pids[i], &estado, 0);
        printf("Worker %d procesó %d líneas.\n", pids[i], lineasProcesadas[i]);
    }

    // Cerrar archivo de entrada
    fclose(archivo);

    // Realizar otras tareas necesarias, como escribir los resultados en el
    // archivo de salida y mostrarlos en la salida estándar si se utiliza la
    // bandera -b

    printf("Proceso broker finalizado.\n");
}