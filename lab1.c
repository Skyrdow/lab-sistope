#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "funciones.h"

int main(int argc, char *argv[])
{
    int option;
    char *input_file_name;
    char *output_file_name;
    int b = 0;
    while ((option = getopt(argc, argv, "i:o:b")) != -1)
    {
        switch (option)
        {
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

    int line_count = getLineCount(input_file_name);
    char **expr_matrix = allocate_matrix(line_count);
    load_regex(input_file_name, expr_matrix);
    int *regex_result_array = process_regex(expr_matrix, line_count);
    create_output_file(output_file_name, expr_matrix, regex_result_array, line_count);

    if (b)
        print_regex_result(regex_result_array, line_count);

    return 0;
}