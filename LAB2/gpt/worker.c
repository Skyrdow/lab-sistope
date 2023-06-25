#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "fworker.h"

int main(char argc, char* argv[]) {
    // Leer líneas del pipe hasta recibir la señal de FIN
    char linea[59];
    while (1) {
        read(atoi(argv[2]), linea, sizeof(linea));
        printf("Línea recibida: %s\n", linea);
        if (strcmp(linea, "FIN") == 0) {
            break;
        }
        write(atoi(argv[1]), "1", 1);
    }
    // if (strcmp(linea, "FIN") == 0) {

    // }

    // Realizar cálculos necesarios para la línea

    // Almacenar resultados si es necesario

    // Enviar cantidad de líneas procesadas al broker

    printf("Proceso worker finalizado.\n");

    return 0;
}