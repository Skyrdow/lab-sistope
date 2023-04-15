#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "funciones.h"

int main(int argc, char *argv[]){
    int option;
    char *input_file_name = "input.txt";
    char *output_file_name = "out.txt";
    int b = 0;
    while((option = getopt(argc, argv, "i:o:b")) != -1){
        switch(option){
            case 'i':
                input_file_name = optarg;
                break;
            case 'o':
                output_file_name = optarg;
                break;
            case 'b':
                b = 1;
                break;
        }
    }
    if(b){printf("Entrada: %s Salida: %s\n", input_file_name, output_file_name);}

    int line_count = getLineCount(input_file_name);
    char **expr_matrix = allocate_matrix(line_count);
    leer_expr(input_file_name, expr_matrix);
    int *expr_result_array = process_file(expr_matrix, line_count);
    print_results(expr_matrix, expr_result_array, line_count, b);
    create_output_file(output_file_name, expr_matrix, expr_result_array, line_count);
    
    return 0;
}