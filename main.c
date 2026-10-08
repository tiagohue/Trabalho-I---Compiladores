#include <stdio.h>
#include <stdlib.h>
#include "parser.h"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Uso: %s <arquivo_de_entrada>\n", argv[0]);
        return EXIT_FAILURE;
    }

    FILE *file = fopen(argv[1], "r");
    if (!file)
    {
        perror("Erro ao abrir o arquivo de entrada");
        return EXIT_FAILURE;
    }

    parse(file);

    fclose(file);
    return EXIT_SUCCESS;
}