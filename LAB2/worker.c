#include "fworker.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define BUFFERMAX 128

// Entradas: Número de chunks
// Salidas: Estado de salida
// Descripcion: Función main del worker, lee y procesa líneas,
//     y envía los resultados.
int main(int argc, char const* argv[]) {
    // Asignación de variables
    int result = 0;
    int chunks = atoi(argv[1]);
    char fd_Hbuffer[BUFFERMAX];
    char fd_Pbuffer[BUFFERMAX + 1]; // + 1 por warning de compilador
    int cuenta = 0; // contador total lineas
    int isRegex[2] = {0, 0}; // contador de resultados
    int fd[2];
    fd[0] = atoi(argv[3]);
    fd[1] = atoi(argv[2]);

    int fin = 1;

    while (fin) {
        // Leer línea
        read(fd[0], fd_Hbuffer, BUFFERMAX);
        // Si es "FIN" se termina de procesar
        if (strcmp(fd_Hbuffer, "FIN") == 0) {
            fin = 0;
        } else {
            // Procesar
            int esRegex = validate_regex(fd_Hbuffer);
            // aumentar contador de resultado correspondiente
            isRegex[esRegex]++; 

            // Enviar línea procesada
            sprintf(fd_Pbuffer, "%s %s", fd_Hbuffer, esRegex ? "Si" : "No");
            write(fd[1], fd_Pbuffer, BUFFERMAX);

            cuenta++;
        }
    }
    // Enviar valores de cantidad de líneas
    sprintf(fd_Pbuffer, "%i %i %i", cuenta, isRegex[1], isRegex[0]);
    write(fd[1], fd_Pbuffer, BUFFERMAX);

    exit(0);
}