#include "fbroker.h"

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