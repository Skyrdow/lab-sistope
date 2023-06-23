#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "fworker.h"

int main() {
    // Leer líneas del pipe hasta recibir la señal de FIN
    char linea[256];
    while (fgets(linea, sizeof(linea), stdin) != NULL) {
        if (strcmp(linea, "FIN\n") == 0) {
            break;
        }

        // Realizar cálculos necesarios para la línea

        // Almacenar resultados si es necesario
    }

    // Enviar cantidad de líneas procesadas al broker
    // ...

    printf("Proceso worker finalizado.\n");

    return 0;
}
