#include "fbroker.h"

// Entradas: Número de workers
// Salidas: Matriz de pipes
// Descripcion: Crea 2 pipes por cada worker
int **crear_pipes(int workers)
{
    int **pipes;
    pipes = (int**)malloc(workers * 2 * sizeof(int*));
    for (int i = 0; i < workers * 2; i++) {
        pipes[i] = (int*)malloc(2 * sizeof(int));
    }

    for (int i = 0; i < workers * 2; i++) { // 2 por cada worker
        if (pipe(pipes[i]) == -1)
        {
            printf("Error al crear el pipe\n");
            exit(1);
        }
    }
    return pipes;
}


// Entradas: Número de workers, Tamaño de chunk, Matriz de pipes, Matriz de pids
// Descripcion: Crea los workers necesarios y almacena sus pids
void crear_workers(int workers, int chunk_size, int **pipes, int *pids)
{
    for (int i = 0; i < workers; i++) {
        int pid = fork();
        if (pid < 0) {
            printf("Error al crear el proceso hijo\n");
            exit(1);
        } else if (pid == 0) {
            // Proceso hijo ejecuta worker
            close(pipes[2 * i][0]);
            close(pipes[2 * i + 1][1]);

            char fd_Pbuffer[BUFFERMAX];
            char fd_Hbuffer[BUFFERMAX];
            char chunks_buffer[BUFFERMAX];
            sprintf(fd_Pbuffer, "%d", pipes[2 * i][1]);
            sprintf(fd_Hbuffer, "%d", pipes[2 * i + 1][0]);
            sprintf(chunks_buffer, "%d", chunk_size);

            char* argv_exec[] = {"./worker", chunks_buffer, fd_Pbuffer,
                                 fd_Hbuffer, NULL};
            execv("./worker", argv_exec);
        } else {
            // Proceso padre
            pids[i] = pid; // almacenar pid
            close(pipes[i * 2][1]);
            close(pipes[i * 2 + 1][0]);
        }
    }
}

// Entradas: Número de workers, Matriz de pipes, 
//     Contador de líneas aceptadas, Contador de líneas rechazadas,
//     Vector de líneas contadas por worker
// Descripcion: Avisa a los workers el fin del procesamiento y cuenta
//     las líneas aceptadas y rechazadas
void contar_lineas(int workers, int **pipes, int *l_si, int *l_no, int *leidas)
{
    char fd_Hbuffer[BUFFERMAX];
    char fd_Pbuffer[BUFFERMAX];

    sprintf(fd_Hbuffer, "FIN");
    for (int i = 0; i < workers; i++) {
        int args[3];
        // Avisar workers
        write(pipes[i * 2 + 1][1], fd_Hbuffer, BUFFERMAX);
        // Recibir conteo
        read(pipes[i * 2][0], fd_Pbuffer, BUFFERMAX);
        sscanf(fd_Pbuffer, "%i %i %i", &args[0], &args[1], &args[2]);
        leidas[i] = args[0];
        (*l_si) += args[1];
        (*l_no) += args[2];
    }
}

// Entradas: Nombre del archivo de salida, Descriptor de salida de lab2, 
//     Flag b
// Descripcion: Imprime los resultados por la consola
void imprimir_resultados(const char *archivoSalida, int fd_lab2, int print_flag)
{
    FILE *fp_out = fopen(archivoSalida, "r");
    int contador = 0;
    
    char file_buffer[BUFFERMAX];
    // Leer archivo de salida
    while (fgets(file_buffer, BUFFERMAX, fp_out) != NULL) {
        // Imprime si son las últimas 4 líneas
        if (contador < LINEAS_PRINT)
            write(fd_lab2, file_buffer, strlen(file_buffer));
        // Imprime el contenido completo con la flag -b
        else if (print_flag)
            write(fd_lab2, file_buffer, strlen(file_buffer));
        contador++;
    }
    fclose(fp_out);
}

// Entradas: Nombre del archivo de salida, Matriz de pids, 
//     Vector de líneas leídas por cada worker, Descriptor de salida de lab2
// Descripcion: Imprime por consola el trabajo de cada worker
void imprimir_informe_workers(int workers, int *pids, int *leidas, int fd_lab2)
{
    char buffer[BUFFERMAX];
    for (int i = 0; i < workers; i++) {
        sprintf(buffer, "El proceso %i leyo %i lineas\n", pids[i], leidas[i]);
        write(fd_lab2, buffer, strlen(buffer));
    }
}