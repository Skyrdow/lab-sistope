#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <pthread.h>
#include <string.h>

// Semáforo global
extern pthread_mutex_t mutex;

// Estructura compartida para registrar los datos de las expresiones
typedef struct {
    int lineas_leidas;
    int leer;

    int *resultados_regex;
    char **lineas_regex;

    int *lineas_hebras;
} DatosCompartidos;

// Estructura que contiene los argumentos para cada hilo
typedef struct {
    int id;
    int chunk_size;

    FILE* archivo_entrada;
    DatosCompartidos* datos_compartidos;
} HiloArgs;


void* hilo_procesador(void* arg);
DatosCompartidos *init_datos_compartidos(int lineas_totales, int cant_hilos);
int get_line_count(char *input_file_name);
char **allocate_matrix(int line_count);
int comparison_or(char check, char *comparations, int comp_count);
int validate_regex(char *regex);
void print_regex_result(int *result_arr, int line_count);
void create_output_file(char *output_file_name, char **expr_matrix, int *result_arr, int line_count);