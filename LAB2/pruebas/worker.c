#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "funciones.h"

int main(int argc, char const* argv[]) {
    int result = 0;
    int chunks = atoi(argv[1]);
    char fd_Hbuffer[100];
    char fd_Pbuffer[100];
    int cuenta = 0;
    int isRegex[2] = {0, 0};

    int fin = 1;

    while (fin) {
        read(atoi(argv[3]), fd_Hbuffer, sizeof(char) * 100);

        if (strcmp(fd_Hbuffer, "FIN") == 0) {
            fin = 0;
        } else {
            int esRegex = validate_regex(fd_Hbuffer);
            isRegex[esRegex]++;

            sprintf(fd_Pbuffer, "%s %s", fd_Hbuffer, esRegex ? "Si" : "No");
            write(atoi(argv[2]), fd_Pbuffer, sizeof(char) * 100);

            cuenta++;
        }
    }

    sprintf(fd_Pbuffer, "%i", cuenta);

    write(atoi(argv[2]), fd_Pbuffer, sizeof(char) * 100);

    exit(0);
}