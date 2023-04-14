#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define EXPR_MAX 60

int leer_expr(char *input_file_name, char **expr_matrix);
char **allocate_matrix(int line_count);
int comparison_or(char check, char* comparations, int comp_count);
int getLineCount(char *input_file_name);
int test_regex(char *regex, int line_count);
int *process_file(char **expr_matrix, int line_count);
void print_results(char **expr_matrix, int *result_arr, int line_count, int b_flag);
void print_matrix(char** expr_matrix, int line_count, int *result_arr);

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
    if(b){printf("%s %s\n", input_file_name, output_file_name);}

    int line_count = getLineCount(input_file_name);
    char **expr_matrix = allocate_matrix(line_count);
    leer_expr(input_file_name, expr_matrix);
    int *expr_result_array = process_file(expr_matrix, line_count);
    print_results(expr_matrix, expr_result_array, line_count, b);
}

int getLineCount(char *input_file_name)
{
    int line_count = 1;
    FILE *fp = fopen(input_file_name, "r");
    char k;
    while (fscanf(fp, "%c", &k) != EOF)
    {
        if (k == '\n')
            line_count++;
    }
    fclose(fp);
    printf("%d \n", line_count);
    return line_count;
}

int leer_expr(char *input_file_name, char **expr_matrix)
{
    FILE *fp = fopen(input_file_name, "r");
    char k;
    int i = 0, j =0;
    while (fscanf(fp, "%c", &k) != EOF)
    {
        if (k == '\n')
        {
            i = 0;
            j++;
            continue;
        }
        expr_matrix[j][i] = k;
        i++;
    }
    fclose(fp);
    return 1;
}

char **allocate_matrix(int line_count)
{
    char **line_matrix = malloc(sizeof(char *) * line_count);
    for (int i = 0; i < line_count; i++)
    {
        line_matrix[i] = malloc(sizeof(char)*EXPR_MAX);
    }
    return line_matrix;
}


int comparison_or(char check, char* comparations, int comp_count)
{
    for (int i = 0; i < comp_count; i++)
    {
        if (check == comparations[i])
            return 1;
    }
    return 0;
}

int test_regex(char *regex, int line_count)
{
    
    int state = 1;
    int index = 0;
    char acgt[4] = {'A', 'C', 'G', 'T'};
    char act[3] = {'A', 'C', 'T'};
    char acg[3] = {'A', 'C', 'G'};
    char ag[2] = {'A', 'G'};

    while (index < EXPR_MAX)
    {
        char check = regex[index];
        switch (state)
        {
        case 1:
            if (comparison_or(check, act, 3))
                state = 1;
            if (check == 'G')
                state = 2;
            break;
        
        case 2:
            if (check == 'T')
                state = 3;
            if (comparison_or(check, acg, 3))
                state = 1;
            break;
        case 3:
            if (check == 'T')
                state = 3;
            if (check == 'C')
                state = 4;
            if (comparison_or(check, ag, 2))
                state = 1;
            break;
        case 4:
            return 1;
        default:
            printf("Algo malo ocurrió\n");
            return -1;
        }
        index++;
    }
    return 0;
}

int *process_file(char **expr_matrix, int line_count)
{
    int *result_arr = malloc(sizeof(int)*line_count);
    for (int i = 0; i < line_count; i++)
    {
        result_arr[i] = test_regex(expr_matrix[i], line_count);
    }
    return result_arr;
}

void print_results(char **expr_matrix, int *result_arr, int line_count, int b_flag)
{
    print_matrix(expr_matrix, line_count, result_arr);
    if (b_flag)
    {
        int true_count = 0;
        int false_count = 0;
        for (int i = 0; i < line_count; i++)
        {
            result_arr[i] ? true_count++ : false_count++;
        }
        
        printf("Total de expresiones que Si son regulares: %d\n", true_count);
        printf("Total de expresiones que No son regulares: %d\n", false_count);
        printf("Total de lineas leídas: %d\n", line_count);
    }
    
}

void print_matrix(char** expr_matrix, int line_count, int *result_arr)
{
    for (int i = 0; i < line_count; i++)
    {
        for (int j = 0; j < EXPR_MAX; j++)
        {
            printf("%c", expr_matrix[i][j]);
        }
        printf(" %s\n", result_arr[i] ? "si" : "no");
    }
    printf("\n");
}