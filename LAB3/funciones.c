#include "funciones.h"

// Constante que define el tamaño máximo de una expresión regular
#define EXPR_MAX 59
#define BUFFER_MAX 100

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// Entradas: Puntero a estructura HiloArgs con los argumentos del hilo
// Descripcion: Función que será ejecutada por cada hilo para procesar el archivo
void* hilo_procesador(void* arg) {
    // argumentos y variables
    HiloArgs* args = (HiloArgs*)arg;
    int id = args->id;
    FILE* archivo_entrada = args->archivo_entrada;
    DatosCompartidos* dc = args->datos_compartidos;
    int chunk_size = args->chunk_size;
    dc->lineas_hebras[id] = 0;

    // matriz de caracteres leída en el chunk
    char **lineas_chunk = allocate_matrix(chunk_size);
    
    // loop principal de lectura y procesamiento
    while (1)
    {
        for (int i = 0; i < chunk_size; i++)
        {
            lineas_chunk[i][0] = '\0'; // vaciar las lineas de chunk
        }
        
        pthread_mutex_lock(&mutex);
        if (dc->leer == 0) // detener lectura del archivo
        {
            pthread_mutex_unlock(&mutex);
            pthread_exit(NULL);
        }

        // leer archivo de entrada
        int lineas_hilo = 0;
        int comienzo_chunk = dc->lineas_leidas;
        char buffer[BUFFER_MAX];
        
        // loop de lectura de <chunk_size> lineas
        while (1)
        {
            // revisar si se llegó al final del archivo
            if (fgets(buffer, BUFFER_MAX, archivo_entrada) == NULL)
            {
                dc->lineas_leidas += lineas_hilo;
                dc->leer = 0;
                pthread_mutex_unlock(&mutex);
                pthread_exit(NULL);
            }

            if (buffer[0] == '\n')
                continue; // ignorar líneas vacías

            buffer[EXPR_MAX] = '\0'; // truncar línea

            int linea_actual = lineas_hilo + dc->lineas_leidas;
            // guardar linea leida
            strcpy(dc->lineas_regex[linea_actual], buffer);
            strcpy(lineas_chunk[lineas_hilo], buffer);

            lineas_hilo++;
            // print para ver cada linea individual leída
            // printf("id%d, a%d, h%d, %s-\n", id, linea_actual, lineas_hilo, buffer);
            if (lineas_hilo == chunk_size) {
                break; //cambia turno
            }
        }
        dc->lineas_hebras[id] += lineas_hilo;
        dc->lineas_leidas += lineas_hilo;
        pthread_mutex_unlock(&mutex);
        sleep(0.5f); // Simular cambio de contexto

        // procesar regex
        pthread_mutex_lock(&mutex);
        for (int i = comienzo_chunk, j = 0; j < lineas_hilo; i++, j++)
        {
            dc->resultados_regex[i] = validate_regex(lineas_chunk[j]);
        }
        pthread_mutex_unlock(&mutex);
        
    }
}

// Entradas: Número de líneas del archivo y cantidad de hilos
// Salidas: Estructura de datos compartidos inicializada
// Descripcion: Inicializa la estructura de datos compartidos, sus arreglos y sus valores iniciales
DatosCompartidos *init_datos_compartidos(int lineas_totales, int cant_hilos)
{
    DatosCompartidos *datos_compartidos = malloc(sizeof(DatosCompartidos));
    datos_compartidos->lineas_leidas = 0;
    datos_compartidos->leer = 1;
    // arreglo de resultado del procesamiento
    int *resultados_regex = malloc(lineas_totales * sizeof(int));
    datos_compartidos->resultados_regex = resultados_regex;
    // arreglo de lineas por cada hebra
    int *lineas_hebras = malloc(cant_hilos * sizeof(int));
    datos_compartidos->lineas_hebras = lineas_hebras;
    // matriz de lineas del archivo
    char **lineas_regex = allocate_matrix(lineas_totales);
    datos_compartidos->lineas_regex = lineas_regex;
    return datos_compartidos;
}

// Entradas: Nombre del archivo de entrada
// Salidas: Número de líneas del archivo
// Descripcion: Función que cuenta el número de líneas de un archivo, ignora las lineas vacías
int get_line_count(char *input_file_name)
{
    int lineas_count = 0;
    int es_linea_vacia = 1;
    FILE *fp = fopen(input_file_name, "r");
    char caracter;
    while ((caracter = fgetc(fp)) != EOF) {
        // marcar linea vacia
        if (caracter != ' ' && caracter != '\t' && caracter != '\n' && caracter != '\r') {
            es_linea_vacia = 0;
        }
        // fin de linea
        if (caracter == '\n') {
            if (!es_linea_vacia)
                lineas_count++;
            es_linea_vacia = 1;
        }
    }
    // verificador de la ultima linea
    if (!es_linea_vacia) {
        lineas_count++;
    }
    fclose(fp);
    return lineas_count;
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
        line_matrix[i] = malloc(sizeof(char *) * EXPR_MAX);
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
int validate_regex(char *regex)
{
    int state = 1;
    int index = 0;
    char acgt[4] = {'A', 'C', 'G', 'T'};
    char act[3] = {'A', 'C', 'T'};
    char ac[2] = {'A', 'C'};

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
            if (check == 'G')
                state = 2;
            if (comparison_or(check, ac, 2))
                state = 1;
            break;
        case 3:
            if (check == 'T')
                state = 3;
            if (check == 'C')
                state = 4;
            if (check == 'A')
                state = 1;
            if (check == 'G')
                state = 2;
            break;
        case 4:
            if (comparison_or(check, acgt, 4))
                state = 4;
            break;
        default:
            return 0;
        }
        index++;
    }

    if (state == 4)
        return 1;
    return 0;
}

// Entradas: Matriz de expresiones regulares, arreglo de resultados,
//  número de líneas del archivo, bandera -b
// Salidas: N/A
// Descripcion: Función que imprime los resultados
void print_regex_result(int *result_arr, int line_count)
{
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