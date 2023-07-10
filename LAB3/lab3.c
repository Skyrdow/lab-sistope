#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include "funciones.h"
#include <pthread.h>
#include <string.h>

#define EXPR_MAX 59
#define BUFFER_MAX 128

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// Estructura compartida para registrar los datos de las expresiones
typedef struct {
    int lineas_leidas;
    int leer;
    int *resultados_regex;
    char **lineas_regex;
} DatosCompartidos;

// Estructura que contiene los argumentos para cada hilo
typedef struct {
    int id;
    FILE* archivo_entrada;
    DatosCompartidos* datos_compartidos;
    int chunk_size;
} HiloArgs;


// Función que será ejecutada por cada hilo
void* hilo_procesador(void* arg) {
    // variables
    HiloArgs* args = (HiloArgs*)arg;
    int id = args->id;
    FILE* archivo_entrada = args->archivo_entrada;
    DatosCompartidos* dc = args->datos_compartidos;
    int chunk_size = args->chunk_size;

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
            printf("id%d, a%d, h%d, %s-\n", id, linea_actual, lineas_hilo, buffer);
            if (lineas_hilo == chunk_size) {
                break; //cambia turno
            }
        }
        
        dc->lineas_leidas += lineas_hilo;
        pthread_mutex_unlock(&mutex);
        sleep(0.5f); // Simular cambio de contexto para pocos hilos

        // procesar regex
        pthread_mutex_lock(&mutex);
        for (int i = comienzo_chunk, j = 0; j < lineas_hilo; i++, j++)
        {
            dc->resultados_regex[i] = validate_regex(lineas_chunk[j]);
        }
        pthread_mutex_unlock(&mutex);
        
    }
}

int main(int argc, char *argv[])
{
    // Validar argumentos de línea de comando
    int option;
    char* archivo_entrada = NULL;
    char* archivo_salida = NULL;
    int cant_hilos = 0;
    int chunk_size = 0;
    int b = 0;

    while ((option = getopt(argc, argv, "i:o:n:c:b")) != -1) {
        switch (option) {
            case 'i':
                archivo_entrada = optarg;
                break;
            case 'o':
                archivo_salida = optarg;
                break;
            case 'n':
                cant_hilos = atoi(optarg);
                break;
            case 'c':
                chunk_size = atoi(optarg);
                break;
            case 'b':
                b = 1;
                break;
        }
    }

    if (archivo_entrada == NULL) {
        printf("Error: falta nombre de archivo de entrada\n");
        return 1;
    }
    if (archivo_salida == NULL) {
        printf("Error: falta nombre de archivo de salida\n");
        return 1;
    }
    if (cant_hilos <= 0) {
        printf("Error: falta cantidad de hilos\n");
        return 1;
    }
    if (chunk_size <= 0) {
        printf("Error: falta tamaño de chunk\n");
        return 1;
    }

    if (b != 0 && b != 1) {
        printf("Error: valor de b incorrecto\n");
        return 1;
    }

    FILE* file = fopen(archivo_entrada, "r");
    if (file == NULL) {
        printf("No se pudo abrir el archivo de entrada.\n");
        return 1;
    }
    
    // estructura de datos compartidos
    DatosCompartidos datos_compartidos;
    datos_compartidos.lineas_leidas = 0;
    datos_compartidos.leer = 1;
    int lineas_totales = get_line_count(archivo_entrada);
    // arreglo de resultado del procesamiento
    int *resultados_regex = malloc(lineas_totales * sizeof(int));
    datos_compartidos.resultados_regex = resultados_regex;
    // matriz de lineas del archivo
    char **lineas_regex = allocate_matrix(lineas_totales);
    datos_compartidos.lineas_regex = lineas_regex;
    
    // crear argumentos
    HiloArgs* args_hilos = malloc(cant_hilos * sizeof(HiloArgs));
    for (int i = 0; i < cant_hilos; i++) {
        args_hilos[i].id = i;
        args_hilos[i].archivo_entrada = file;
        args_hilos[i].datos_compartidos = &datos_compartidos;
        args_hilos[i].chunk_size = chunk_size;
    }
    
    // crear hilos
    pthread_t* hilos = malloc(cant_hilos * sizeof(pthread_t));
    for (int i = 0; i < cant_hilos; i++) {
        pthread_create(&hilos[i], NULL, hilo_procesador, (void*)&args_hilos[i]);
    }
    
    // esperar ejecución de los hilos
    for (int i = 0; i < cant_hilos; i++) {
        pthread_join(hilos[i], NULL);
    }
    
    // printf("%d, %d, %d\n", cant_hilos, chunk_size, lineas_totales);
    free(args_hilos);
    fclose(file);
    
    create_output_file(archivo_salida, lineas_regex, resultados_regex, lineas_totales);
    if (b)
        print_regex_result(resultados_regex, lineas_totales);
    
    return 0;
}