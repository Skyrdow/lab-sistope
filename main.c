#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define EXPR_MAX 60

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

    FILE *fp = fopen(input_file_name, "r");
    char k;

    int line_count = 9;

    char **line_matrix = malloc(sizeof(char *) * line_count);
    for (int i = 0; i < line_count; i++)
    {
        line_matrix[i] = malloc(sizeof(char)*EXPR_MAX);
    }
    
    int i = 0, j =0;
    while (fscanf(fp, "%c", &k) != EOF)
    {
        if (k == '\n')
        {
            i = 0;
            j++;
            continue;
        }
        line_matrix[j][i] = k;
        i++;
    }

    for (int i = 0; i < line_count; i++)
    {
        for (int j = 0; j < EXPR_MAX; j++)
        {
            printf("%c", line_matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
    

    fclose(fp);
    return 0;
}
