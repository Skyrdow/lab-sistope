#include "funciones.h"

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
    int lineas_totales = get_line_count(archivo_entrada);
    DatosCompartidos *dc = init_datos_compartidos(lineas_totales, cant_hilos);
    
    // crear argumentos
    HiloArgs* args_hilos = malloc(cant_hilos * sizeof(HiloArgs));
    for (int i = 0; i < cant_hilos; i++) {
        args_hilos[i].id = i;
        args_hilos[i].archivo_entrada = file;
        args_hilos[i].datos_compartidos = dc;
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

    free(args_hilos);
    free(hilos);
    fclose(file);
    
    create_output_file(archivo_salida, dc->lineas_regex, dc->resultados_regex, lineas_totales);

    if (!b) // si no está -b, terminar
    {
        free(dc);
        return 0;
    }

    print_regex_result(dc->resultados_regex, lineas_totales);
    for (int i = 0; i < cant_hilos; i++)
    {
        printf("La hebra %d leyó %d lineas\n", i, dc->lineas_hebras[i]);
    }

    free(dc);
    return 0;
}