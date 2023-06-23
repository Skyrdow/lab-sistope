#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "funciones.h"

int main(int argc, char* argv[]) {
    int option;
    char* input_file_name;
    char* output_file_name;
    int workers;
    int chunkSize;
    int b = 0;
    while ((option = getopt(argc, argv, "i:o:w:c:b")) != -1) {
        switch (option) {
            case 'i':
                if (!optarg) {
                    printf("Error: el argumento en -i es requerido\n");
                    return 1;
                }
                input_file_name = optarg;
                printf("El nombre del archivo es: %s\n", input_file_name);
                break;
            case 'o':
                if (!optarg) {
                    printf("Error: el argumento en -o es requerido\n");
                    return 1;
                }
                output_file_name = optarg;
                break;
            case 'w':
                if (!optarg) {
                    printf("Error: el argumento en -w es requerido\n");
                    return 1;
                }
                workers = atoi(optarg);
                break;
            case 'c':
                if (!optarg) {
                    printf("Error: el argumento en -c es requerido\n");
                    return 1;
                }
                chunkSize = atoi(optarg);
                break;
            case 'b':
                b = 1;
                break;
        }
    }

    int pid;
    // 0 leer 1 escribir

    pid = fork();

    if (pid == -1)
        return -1;
    else if (pid == 0) {
        // Hijo
        char fd_buffer[100];
        char chunk_buffer[100];
        char workers_buffer[100];
        char b_buffer[100];
        sprintf(fd_buffer, "%d", fd[1]);
        sprintf(chunk_buffer, "%d", chunkSize);
        sprintf(workers_buffer, "%d", workers);
        sprintf(b_buffer, "%d", b);
        
        execlp("./fbroker", "./fbroker", input_file_name, output_file_name,
               chunk_buffer, workers_buffer, b_buffer, NULL);
    }

    // int line_count = getLineCount(input_file_name);
    // char** expr_matrix = allocate_matrix(line_count);
    // load_regex(input_file_name, expr_matrix);
    // int* regex_result_array = process_regex(expr_matrix, line_count);
    // create_output_file(output_file_name, expr_matrix, regex_result_array,
    //                    line_count);

    // if (b)
    //     print_regex_result(regex_result_array, line_count);
    return 0;
}