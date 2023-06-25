#include "fworker.h"

void procesarLineas() {
    // Leer líneas del pipe hasta recibir la señal de FIN
    char linea[EXPR_MAX];
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
