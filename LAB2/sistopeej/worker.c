#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
    int result = atoi(argv[1]) + atoi(argv[2]);
    char buffer[100];
    sprintf(buffer, "%i", result);
    // write(STDOUT_FILENO, &result, sizeof(int)); works
    dup2(STDOUT_FILENO, atoi(argv[3]));
    write(atoi(argv[3]), &result, sizeof(int));
    return 0;
}
