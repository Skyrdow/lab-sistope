#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
    int pid;
    int fd[2];

    pid = fork();
    pipe(fd);

    printf("El pid es: %d\n", pid);
    printf("Los pipe estan en %i, %i\n", fd[0], fd[1]);

    if (pid == 0) {
        printf("Soy el hijo\n");
    } else {
        printf("Soy el padre\n");
    }

    return 0;
}
