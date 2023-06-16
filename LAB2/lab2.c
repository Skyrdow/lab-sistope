#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
// #include "funciones.h"

int main(int argc, char *argv[])
{
    int option;
    char *input_file_name;
    char *output_file_name;
    int chunk_size;
    int workers;
    int b = 0;
    while ((option = getopt(argc, argv, "i:o:c:n:b")) != -1)
    {
        switch (option)
        {
        case 'i':
            input_file_name = optarg;
            break;
        case 'o':
            output_file_name = optarg;
            break;
        case 'c':
            chunk_size = atoi(optarg);
            break;
        case 'n':
            workers = atoi(optarg);
            break;
        case 'b':
            b = 1;
            break;
        }
    }

    // Revisar correcto
    printf("%s\n", input_file_name);
    printf("%s\n", output_file_name);
    printf("%d\n", chunk_size);
    printf("%d\n", workers);

    // pipe
    int fd[2];
    pipe(fd);

    int pid;

    pid = fork();
    if (pid == -1)
        return -1;

    // Si el pid es 0 significa que es el proceso hijo
    if(pid == 0){
        dup2(STDIN_FILENO, fd[0]);
        char fd_buffer[100];
        char chunk_buffer[100];
        char wokers_buffer[100];
        char b_buffer[100];
        sprintf(fd_buffer, "%d", fd[1]);
        sprintf(chunk_buffer, "%d", chunk_size);
        sprintf(wokers_buffer, "%d", workers);
        sprintf(b_buffer, "%d", b);

        char* argv_exec[] = {"./broker", input_file_name, output_file_name, chunk_buffer, wokers_buffer, b_buffer, fd_buffer, NULL};
        execv("./broker", argv_exec);
    }
    // Si el pid es > a 0 significa que es el proceso padre
    else{
        char buffer[100];
        close(fd[1]);
        read(fd[0], buffer, sizeof(int));
        printf("Soy el padre y creé el broker%s\n", buffer);
    }
    return 0;
}