#include <stdio.h>
#include <stdlib.h>
// #include <unistd.h>

#define EXPR_MAX 60

int leer_archivo(char *input_file_name, char **expr_matrix);
char **allocate_matrix(int line_count);
void print_matrix(char** expr_matrix, int line_count);
int comparison_or(char check, char* comparations, int comp_count);
int test_regex(char **expr_matrix);

int main(int argc, char *argv[]){
    int option;
    char *input_file_name = "input.txt";
    char *output_file_name = "out.txt";
    int b = 0;
    // while((option = getopt(argc, argv, "i:o:b")) != -1){
    //     switch(option){
    //         case 'i':
    //             input_file_name = optarg;
    //             break;
    //         case 'o':
    //             output_file_name = optarg;
    //             break;
    //         case 'b':
    //             b = 1;
    //             break;
    //     }
    // }
    // if(b){printf("%s %s\n", input_file_name, output_file_name);}


    int line_count = 9;
    char **expr_matrix = allocate_matrix(line_count);
    int *expr_bool_array = malloc(sizeof(int) * line_count);
    leer_archivo(input_file_name, expr_matrix);
    print_matrix(expr_matrix, line_count);
    printf("%d", test_regex(expr_matrix));
}

int leer_archivo(char *input_file_name, char **expr_matrix)
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

void print_matrix(char** expr_matrix, int line_count)
{
    for (int i = 0; i < line_count; i++)
    {
        for (int j = 0; j < EXPR_MAX; j++)
        {
            printf("%c", expr_matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
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

int test_regex(char **expr_matrix)
{
    
    int state = 1;
    int index = 0;
    char acgt[4] = {'A', 'C', 'G', 'T'};
    char act[3] = {'A', 'C', 'T'};
    char acg[3] = {'A', 'C', 'G'};
    char ag[2] = {'A', 'G'};

    while (index < EXPR_MAX)
    {
        char check = expr_matrix[4][index];
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