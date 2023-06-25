#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "funciones.h"

#define BUFFERMAX 128

int main(int argc, char const* argv[]) {
    int result = 0;
    int chunks = atoi(argv[1]);
    char fd_Hbuffer[BUFFERMAX];
    char fd_Pbuffer[BUFFERMAX + 1]; // + 1 por warning de compilador
    int cuenta = 0;
    int isRegex[2] = {0, 0};

    int fin = 1;

    while (fin) {
        read(atoi(argv[3]), fd_Hbuffer, BUFFERMAX);

        if (strcmp(fd_Hbuffer, "FIN") == 0) {
            fin = 0;
        } else {
            int esRegex = validate_regex(fd_Hbuffer);
            isRegex[esRegex]++;

            sprintf(fd_Pbuffer, "%s %s", fd_Hbuffer, esRegex ? "Si" : "No");
            write(atoi(argv[2]), fd_Pbuffer, BUFFERMAX);

            cuenta++;
        }
    }

    sprintf(fd_Pbuffer, "%i %i %i", cuenta, isRegex[1], isRegex[0]);

    write(atoi(argv[2]), fd_Pbuffer, BUFFERMAX);

    exit(0);
}