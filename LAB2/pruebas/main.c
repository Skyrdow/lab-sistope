#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char const* argv[]) {
    const char* archivoEntrada = argv[1];
    const char* archivoSalida = argv[2];
    int workers = atoi(argv[3]);
    int chunkSize = atoi(argv[4]);;
    int pipes[workers * 2][2];

    for (int i = 0; i < workers * 2; i++) {
        pipe(pipes[i]);
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

            char fd_Pbuffer[100];
            char fd_Hbuffer[100];
            char chunks_buffer[100];
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

    char fd_Hbuffer[100];
    char fd_Pbuffer[100];

    FILE* fp = fopen("prueba_10.txt", "r");
    FILE* fp2 = fopen("salida.txt", "w");
    int turno = 0;
    int fin[workers];
    memset(fin, 0, sizeof(int) * workers);
    while (fgets(fd_Hbuffer, 100, fp) != NULL) {
        if (fd_Hbuffer[0] == '\n') {
            continue;
        }
        size_t len = strcspn(fd_Hbuffer, "\n");
        if (fd_Hbuffer[len] == '\n') {
            // Reemplazar '\n' con '\0' para terminar la cadena
            fd_Hbuffer[len] = '\0';
        }
        // printf("Enviando %s\n", fd_Hbuffer);
        write(pipes[turno * 2 + 1][1], fd_Hbuffer, sizeof(char) * 100);
        read(pipes[turno * 2][0], fd_Pbuffer, sizeof(char) * 100);
        fprintf(fp2, "%s\n", fd_Pbuffer);
        fin[turno]++;
        if (fin[turno] == chunkSize) {
            fin[turno] = 0;
            turno = (turno + 1) % workers;
        }
    }
    fclose(fp);

    sprintf(fd_Hbuffer, "FIN");
    int leidas[workers];
    memset(leidas, 0, sizeof(int) * workers);
    for (int i = 0; i < workers; i++) {
        write(pipes[i * 2 + 1][1], fd_Hbuffer, sizeof(char) * 100);
        read(pipes[i * 2][0], fd_Pbuffer, sizeof(char) * 100);
        leidas[i] += atoi(fd_Pbuffer);
    }
    close(pipes[1][1]);
    close(pipes[0][0]);

    for (int i = 0; i < workers; i++) {
        printf("Soy proceso %i y lei %i lineas\n", pids[i], leidas[i]);
        fprintf(fp2, "Soy proceso %i y lei %i lineas\n", pids[i], leidas[i]);
    }
    fclose(fp2);

    return 0;
}