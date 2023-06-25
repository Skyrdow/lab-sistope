#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#define EXPR_MAX 60

void procesarArchivo(const char *archivoEntrada, const char *archivoSalida, int cantidadWorkers, int numeroChunks);
