#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include "funciones.h"
#include <pthread.h>

#define EXPR_MAX 59
#define BUFFER_MAX 100

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// Estructura compartida para registrar los datos de las expresiones
typedef struct {
    // Aquí puedes agregar los campos necesarios para mantener los datos de las expresiones
    int lineasLeidas;
    int **resultados_regex;
} DatosCompartidos;

// Estructura que contiene los argumentos para cada hilo
typedef struct {
    FILE* archivoEntrada;
    DatosCompartidos* datosCompartidos;
    int chunkSize;
} HiloArgs;


// Función que será ejecutada por cada hilo
void* procesarArchivo(void* arg) {
    HiloArgs* args = (HiloArgs*)arg;
    FILE* archivoEntrada = args->archivoEntrada;
    DatosCompartidos* datosCompartidos = args->datosCompartidos;
    int chunkSize = args->chunkSize;
    
    char linea[EXPR_MAX];
    
    // Leer chunkSize líneas del archivo hasta que no haya más líneas
    while (fgets(linea, sizeof(linea), archivoEntrada) != NULL) {
        // Bloquear el acceso exclusivo al archivo mientras se lee una línea
        // Este bloqueo se realiza utilizando un mutex
        // Aquí puedes agregar cualquier otra lógica necesaria para sincronizar el acceso al archivo
        pthread_mutex_lock(&mutex);
        // Sección crítica: código donde se accede a la estructura compartida
        // Leer archivo de entrada
        
        char buffer[BUFFER_MAX];
        while (fgets(buffer, BUFFER_MAX, archivoEntrada) != NULL) {

            if (buffer[0] == '\n')
                continue; // ignorar líneas vacías

            buffer[59] = '\0'; // Truncar línea

            // fin[turno]++;
            // if (fin[turno] == chunkSize) {
            //     fin[turno] = 0;
            //     turno = (turno + 1) % workers;
            // }
        }
        // Actualizar los datos de la expresión correspondiente
        // Aquí debes implementar la lógica para procesar la expresión regular y actualizar los datos
        
        // Desbloquear el acceso exclusivo al archivo
        // Aquí debes liberar el mutex utilizado para el bloqueo anteriormente
        // Puedes agregar cualquier otra lógica necesaria para sincronizar el acceso al archivo
        pthread_mutex_unlock(&mutex);
        
        lineasLeidas++;
        
        // Si se ha leído el número máximo de líneas definido por chunkSize, terminar la lectura
        if (lineasLeidas == chunkSize) {
            break;
        }
    }
    
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    // Validar argumentos de línea de comando
    int option;
    char* archivoEntrada = NULL;
    char* archivoSalida = NULL;
    int workers = 0;
    int chunkSize = 0;
    int b = 0;
    while ((option = getopt(argc, argv, "i:o:n:c:b")) != -1) {
        switch (option) {
            case 'i':
                archivoEntrada = optarg;
                break;
            case 'o':
                archivoSalida = optarg;
                break;
            case 'n':
                workers = atoi(optarg);
                break;
            case 'c':
                chunkSize = atoi(optarg);
                break;
            case 'b':
                b = 1;
                break;
        }
    }

    if (archivoEntrada == NULL) {
        printf("Error: falta nombre de archivo de entrada\n");
        return 1;
    }
    if (archivoSalida == NULL) {
        printf("Error: falta nombre de archivo de salida\n");
        return 1;
    }
    if (workers <= 0) {
        printf("Error: falta cantidad de workers\n");
        return 1;
    }
    if (chunkSize <= 0) {
        printf("Error: falta tamaño de chunk\n");
        return 1;
    }

    if (b != 0 && b != 1) {
        printf("Error: valor de b incorrecto\n");
        return 1;
    }

    FILE* file = fopen(archivoEntrada, "r");
    if (file == NULL) {
        printf("No se pudo abrir el archivo de entrada.\n");
        return 1;
    }
    
    // Crear las estructuras compartidas para los datos de las expresiones
    DatosCompartidos datosCompartidos;
    datosCompartidos.lineasLeidas = 0;
    
    // Aquí debes inicializar los campos de la estructura con los valores adecuados
    
    // Crear los argumentos para cada hilo
    HiloArgs* hiloArgs = malloc(workers * sizeof(HiloArgs));
    for (int i = 0; i < workers; i++) {
        hiloArgs[i].archivoEntrada = file;
        hiloArgs[i].datosCompartidos = &datosCompartidos;
        hiloArgs[i].chunkSize = chunkSize;
    }
    
    // Crear los hilos
    pthread_t* hilos = malloc(workers * sizeof(pthread_t));
    for (int i = 0; i < workers; i++) {
        pthread_create(&hilos[i], NULL, procesarArchivo, (void*)&hiloArgs[i]);
    }
    
    // Esperar a que todos los hilos terminen
    for (int i = 0; i < workers; i++) {
        pthread_join(hilos[i], NULL);
    }
    
    // Realizar cualquier procesamiento adicional o escribir los resultados en el archivo de salida
    
    // Liberar memoria y cerrar archivos
    free(hiloArgs);
    fclose(file);
    
    return 0;

    int line_count = getLineCount(archivoEntrada);
    char **expr_matrix = allocate_matrix(line_count);
    load_regex(archivoEntrada, expr_matrix);
    int *regex_result_array = process_regex(expr_matrix, line_count);
    create_output_file(archivoSalida, expr_matrix, regex_result_array, line_count);

    if (b)
        print_regex_result(regex_result_array, line_count);

    return 0;
}