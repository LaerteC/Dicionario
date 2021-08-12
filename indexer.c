#include <math.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include "bibliotecaStruct.h"
#include "bibliotecaFreq.h"

// Main para execução !

// Alunos : Laerte Souza e Gustavo de Oliveira.
// Professor Dr: Alexander Robert Kutzke.


int main(int argc, char *argv[])
{

    switch (verificaArgumento(argv[1]))
    {
    case 1:

        freq(argv);
        break;
    case 2:

        freq_word(argv);
        break;
    case 3:
        search(argv);

    default:
        printf("Verificar o parametro passado !\n");
        break;
    }

    return 0;
}