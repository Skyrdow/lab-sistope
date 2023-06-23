#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int load_regex(char* input_file_name, char** expr_matrix);
char** allocate_matrix(int line_count);
int comparison_or(char check, char* comparations, int comp_count);
int getLineCount(char* input_file_name);
int validate_regex(char* regex);
int* process_regex(char** expr_matrix, int line_count);
void print_regex_result(int* result_arr, int line_count);
void create_output_file(char* output_file_name,
                        char** expr_matrix,
                        int* result_arr,
                        int line_count);