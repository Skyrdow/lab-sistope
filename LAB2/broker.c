#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "fbroker.h"

#define BUFFERMAX 128
#define LINEAS_PRINT 4


// Entradas: Archivo de entrada, Archivo de salida, Numero de trabajadores,
//     Tamaño de chunk, Descriptor de la salida estándar del padre lab2,
//     Flag de impresión.
// Salidas: Estado de salida
// Descripcion: Función main de broker
int main(int argc, char const* argv[]) {
    // Asignación de variables
    const char* archivoEntrada = argv[1];
    const char* archivoSalida = argv[2];
    int workers = atoi(argv[3]);
    int chunkSize = atoi(argv[4]);
    int fd_lab2 = atoi(argv[5]);
    int print_flag = atoi(argv[6]);
    int **pipes;

    // Creación de pipes
    pipes = crear_pipes(workers);

    int pids[workers]; // pids de los workers

    // Procesar en cada worker
    crear_workers(workers, chunkSize, pipes, pids);

    char fd_Hbuffer[BUFFERMAX];
    char fd_Pbuffer[BUFFERMAX];

    // Abrir archivos
    FILE* fp_in = fopen(archivoEntrada, "r");
    FILE* fp_out = fopen(archivoSalida, "w");
    int turno = 0;
    int fin[workers];
    memset(fin, 0, sizeof(int) * workers); // inicializar valores de fin en 0

    // Leer archivo de entrada
    while (fgets(fd_Hbuffer, BUFFERMAX, fp_in) != NULL) {

        if (fd_Hbuffer[0] == '\n')
            continue; // ignorar líneas vacías

        fd_Hbuffer[59] = '\0'; // Truncar línea

        // Enviar línea al worker
        write(pipes[turno * 2 + 1][1], fd_Hbuffer, BUFFERMAX);
        // Recibir línea procesada del worker
        read(pipes[turno * 2][0], fd_Pbuffer, BUFFERMAX);
        // Escribir en el archivo de salida
        fprintf(fp_out, "%s\n", fd_Pbuffer);

        fin[turno]++;
        if (fin[turno] == chunkSize) {
            fin[turno] = 0;
            turno = (turno + 1) % workers;
        }
    }
    fclose(fp_in);
    
    // Contar líneas
    int leidas[workers];
    int lineas_total;
    int lineas_si = 0;
    int lineas_no = 0;

    contar_lineas(workers, pipes, &lineas_si, &lineas_no, leidas);
    lineas_total = lineas_si + lineas_no;

    close(pipes[1][1]);
    close(pipes[0][0]);
    
    // Escribir en el archivo e imprimir resultados
    imprimir_resultados(archivoSalida, fd_lab2);
    // Cerrar archivo
    fclose(fp_out);

    if (print_flag)
        imprimir_informe_workers(workers, pids, leidas, fd_lab2);

    return 0;
}