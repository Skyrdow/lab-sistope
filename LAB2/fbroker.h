#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define BUFFERMAX 128
#define LINEAS_PRINT 4

int **crear_pipes(int workers);
void crear_workers(int workers, int chunkSize, int **pipes, int *pids);
void contar_lineas(int workers, int **pipes, int *l_si, int *l_no, int *leidas);
void imprimir_resultados(FILE *fp_out, int fd_lab2, int lineas_si, int lineas_no, int lineas_total);
void imprimir_informe_workers(int workers, int *pids, int *leidas, int fd_lab2);