#include <unistd.h>
#include <stdio.h>

main()
{
    int i = 0;
    int pid1 = fork();
    i = i + 1;
    int pid2 = fork();
    i = i + 2;
    int pid3 = fork();
    if (pid2 > 0 && pid1 != 0)
    {
        printf("asd%d\n", i);
    }
    else
    {
        printf(" %d\n", i+2);
    }
}
