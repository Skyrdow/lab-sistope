#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "funciones.h"

// Constante que define el tamaño máximo de una expresión regular
#define EXPR_MAX 60

// Entradas: Nombre del archivo de entrada
// Salidas: Número de líneas del archivo
// Descripcion: Función que cuenta el número de líneas de un archivo
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
    return line_count;
}

// Entradas: Nombre del archivo de entrada, matriz de expresiones regulares
// Salidas: 1 si la lectura fue exitosa, -1 si hubo un error
// Descripcion: Función que lee el archivo de entrada y
// guarda las expresiones regulares en la matriz
int leer_expr(char *input_file_name, char **expr_matrix)
{
    FILE *fp = fopen(input_file_name, "r");
    char k;
    int i = 0, j = 0;
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

// Entradas: Número de líneas del archivo
// Salidas: Matriz de expresiones regulares
// Descripcion: Función que reserva memoria
// para la matriz de expresiones regulares
char **allocate_matrix(int line_count)
{
    char **line_matrix = malloc(sizeof(char *) * line_count);
    for (int i = 0; i < line_count; i++)
    {
        line_matrix[i] = malloc(sizeof(char) * EXPR_MAX);
    }
    return line_matrix;
}

// Entradas: Caracter a comparar, arreglo de caracteres a comparar, tamaño del arreglo
// Salidas: 1 si el caracter está en el arreglo, 0 si no
// Descripcion: Función que compara un caracter con un arreglo de caracteres
int comparison_or(char check, char *comparations, int comp_count)
{
    for (int i = 0; i < comp_count; i++)
    {
        if (check == comparations[i])
            return 1;
    }
    return 0;
}

// Entradas: Expresion regular, número de líneas del archivo
// Salidas: 1 si la expresión regular es válida, 0 si no y -1 si hubo un error
// Descripcion: Función que verifica si una expresión regular es válida
int test_regex(char *regex, int line_count)
{

    int state = 1;
    int index = 0;
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

// Entradas: Matriz de expresiones regulares, número de líneas del archivo
// Salidas: Arreglo de resultados de las expresiones regulares
// Descripcion: Función que procesa las expresiones regulares
int *process_file(char **expr_matrix, int line_count)
{
    int *result_arr = malloc(sizeof(int) * line_count);
    for (int i = 0; i < line_count; i++)
    {
        result_arr[i] = test_regex(expr_matrix[i], line_count);
    }
    return result_arr;
}

// Entradas: Matriz de expresiones regulares, arreglo de resultados,
//  número de líneas del archivo, bandera -b
// Salidas: N/A
// Descripcion: Función que imprime los resultados
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

// Entradas: Matriz de expresiones regulares,
// número de líneas del archivo, arreglo de resultados
// Salidas: N/A
// Descripcion: Función que imprime la matriz de expresiones regulares
void print_matrix(char **expr_matrix, int line_count, int *result_arr)
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

// Entradas: Nombre del archivo de salida, matriz de expresiones regulares,
// arreglo de resultados, número de líneas del archivo
// Salidas: N/A
// Descripcion: Función que crea el archivo de salida
void create_output_file(char *output_file_name, char **expr_matrix, int *result_arr, int line_count)
{
    FILE *fp = fopen(output_file_name, "w");
    for (int i = 0; i < line_count; i++)
    {
        for (int j = 0; j < EXPR_MAX; j++)
        {
            fprintf(fp, "%c", expr_matrix[i][j]);
        }
        fprintf(fp, " %s\n", result_arr[i] ? "si" : "no");
    }

    int true_count = 0;
    int false_count = 0;
    for (int i = 0; i < line_count; i++)
    {
        result_arr[i] ? true_count++ : false_count++;
    }

    fprintf(fp, "Total de expresiones que Si son regulares: %d\n", true_count);
    fprintf(fp, "Total de expresiones que No son regulares: %d\n", false_count);
    fprintf(fp, "Total de lineas leídas: %d\n", line_count);
    
    fclose(fp);
}