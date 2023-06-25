#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "fbroker.h"

#define BUFFERMAX 128

int main(int argc, char const* argv[]) {
    const char* archivoEntrada = argv[1];
    const char* archivoSalida = argv[2];
    int workers = atoi(argv[3]);
    int chunkSize = atoi(argv[4]);
    int fd_lab2 = atoi(argv[5]);
    int print_flag = atoi(argv[6]);
    int pipes[workers * 2][2];

    for (int i = 0; i < workers * 2; i++) {
        if (pipe(pipes[i]) == -1)
        {
            printf("Error al crear el pipe\n");
            exit(1);
        }
    }

    int pids[workers];

    for (int i = 0; i < workers; i++) {
        int pid = fork();
        if (pid < 0) {
            printf("Error al crear el proceso hijo\n");
            exit(1);
        } else if (pid == 0) {
            close(pipes[2 * i][0]);
            close(pipes[2 * i + 1][1]);

            char fd_Pbuffer[BUFFERMAX];
            char fd_Hbuffer[BUFFERMAX];
            char chunks_buffer[BUFFERMAX];
            sprintf(fd_Pbuffer, "%d", pipes[2 * i][1]);
            sprintf(fd_Hbuffer, "%d", pipes[2 * i + 1][0]);
            sprintf(chunks_buffer, "%d", chunkSize);

            char* argv_exec[] = {"./worker", chunks_buffer, fd_Pbuffer,
                                 fd_Hbuffer, NULL};
            execv("./worker", argv_exec);
        } else {
            pids[i] = pid;
            close(pipes[i * 2][1]);
            close(pipes[i * 2 + 1][0]);
        }
    }
    int suma;

    char fd_Hbuffer[BUFFERMAX];
    char fd_Pbuffer[BUFFERMAX];

    FILE* fp_in = fopen(archivoEntrada, "r");
    FILE* fp_out = fopen(archivoSalida, "w");
    int turno = 0;
    int fin[workers];
    memset(fin, 0, sizeof(int) * workers);
    while (fgets(fd_Hbuffer, BUFFERMAX, fp_in) != NULL) {
        if (fd_Hbuffer[0] == '\n') {
            continue;
        }

        // char temp_buffer[BUFFERMAX];
        // strncpy(temp_buffer, fd_Hbuffer, 59);
        // temp_buffer[59] = '\0';
        // strcpy(fd_Hbuffer, temp_buffer);

        fd_Hbuffer[59] = '\0';

        write(pipes[turno * 2 + 1][1], fd_Hbuffer, BUFFERMAX);
        read(pipes[turno * 2][0], fd_Pbuffer, BUFFERMAX);

        fprintf(fp_out, "%s\n", fd_Pbuffer);
        fin[turno]++;
        if (fin[turno] == chunkSize) {
            fin[turno] = 0;
            turno = (turno + 1) % workers;
        }
    }
    fclose(fp_in);

    sprintf(fd_Hbuffer, "FIN");
    int lineas_total = 0;
    int lineas_si = 0;
    int lineas_no = 0;
    for (int i = 0; i < workers; i++) {
        int leidas[3];
        write(pipes[i * 2 + 1][1], fd_Hbuffer, BUFFERMAX);
        read(pipes[i * 2][0], fd_Pbuffer, BUFFERMAX);

        sscanf(fd_Pbuffer, "%i %i %i", &leidas[0], &leidas[1], &leidas[2]);
        // printf("%s ", fd_Pbuffer);
        // printf("%i %i %i", leidas[i][0], leidas[i][2], leidas[i][3]);

        lineas_total += leidas[0];
        lineas_si += leidas[1];
        lineas_no += leidas[2];
    }
    close(pipes[1][1]);
    close(pipes[0][0]);
    
    fprintf(fp_out, "Total de expresiones que Si son regulares: %d\n", lineas_si);
    fprintf(fp_out, "Total de expresiones que No son regulares: %d\n", lineas_no);
    fprintf(fp_out, "Total de lineas leídas: %d\n", lineas_total);
    
    fclose(fp_out);
    if (print_flag)
    {
        fp_out = fopen(archivoSalida, "r");
        
        char file_buffer[BUFFERMAX];
        while (fgets(file_buffer, BUFFERMAX, fp_out) != NULL) {
            write(fd_lab2, file_buffer, strlen(file_buffer));
        }

        fclose(fp_out);
    }

    return 0;
}