#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
    // pipe
    int fd[2];
    pipe(fd);

    int pid;

    pid = fork();

    // Si el pid es 0 significa que es el proceso hijo
    if(pid == 0){
        // close(fd[0]);
        // dup2(fd[1], STDOUT_FILENO); works
        dup2(STDIN_FILENO, fd[0]);
        char fd_buffer[100];
        sprintf(fd_buffer, "%d", fd[1]);

        char* argv_exec[] = {"./worker", "2", "3", fd_buffer, NULL};
        execv("./worker", argv_exec);
    }
    // Si el pid es > a 0 significa que es el proceso padre
    else{
        char buffer[100];
        int suma;
        close(fd[1]);
        read(fd[0], &suma, sizeof(int));
        printf("Soy el padre y la suma es %d\n", suma);
    }
    return 0;
}
