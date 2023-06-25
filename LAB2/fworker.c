#include "fworker.h"

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
