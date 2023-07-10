#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int get_line_count(char *input_file_name);
char **allocate_matrix(int line_count);
int comparison_or(char check, char *comparations, int comp_count);
int validate_regex(char *regex);
void print_regex_result(int *result_arr, int line_count);
void create_output_file(char *output_file_name, char **expr_matrix, int *result_arr, int line_count);