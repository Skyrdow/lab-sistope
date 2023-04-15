int leer_expr(char *input_file_name, char **expr_matrix);
char **allocate_matrix(int line_count);
int comparison_or(char check, char* comparations, int comp_count);
int getLineCount(char *input_file_name);
int test_regex(char *regex, int line_count);
int *process_file(char **expr_matrix, int line_count);
void print_results(char **expr_matrix, int *result_arr, int line_count, int b_flag);
void print_matrix(char** expr_matrix, int line_count, int *result_arr);
void create_output_file(char *output_file_name, char **expr_matrix, int *result_arr, int line_count);