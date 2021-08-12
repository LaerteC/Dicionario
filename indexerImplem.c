#include <math.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include "bibliotecaStruct.h"
#include "bibliotecaFreq.h"
#include <time.h>


// Alunos : Laerte Souza e Gustavo de Oliveira.
// Professor Dr: Alexander Robert Kutzke.

//Uma função de utilitário para criar um novo node Trie
arvoreNode *novaArvoreNode()
{
    // Alocar memória para o node Trie
    arvoreNode *arvoreNo = (arvoreNode *)calloc(1, sizeof(arvoreNode));

    //Inicializar valores para o novo node
    arvoreNo->fim = 0;
    arvoreNo->frequencia = 0;
    arvoreNo->indexMinHeap = -1;
    int i;
    for (i = 0; i < MAX_CARACTER; ++i)
        arvoreNo->children[i] = NULL;

    return arvoreNo;
}

data *criarDados()
{
    data *dados = (data *)calloc(1, sizeof(data *));
    return dados;
}

data *arrayDocs(float qtd_docs)
{
    data *array = (data *)calloc(1, sizeof(data *) * qtd_docs);
    return array;
}

// Uma função de utilitário para criar um heap mínimo da capacidade fornecida
MinHeap *createMinHeap(int capacity)
{
    MinHeap *minHeap = (MinHeap *)calloc(1, sizeof(MinHeap *));

    minHeap->capacity = capacity;
    minHeap->count = 0;

    // Aloca memória para a matriz de nós de heap mínimo
    minHeap->array = (MinHeapNode *)calloc(1, sizeof(MinHeapNode) * capacity); //novo MinHeapNode

    return minHeap;
}

// Uma função de utilitário para trocar dois nós de heap mínimo. Esta função
// é necessário no minHeapify
void swapMinHeapNodes(MinHeapNode *a, MinHeapNode *b)
{
    MinHeapNode temp = *a;
    *a = *b;
    *b = temp;
}

// Esta é a função minHeapify padrão. Faz uma coisa extra.
// Atualiza o minHapIndex em Trie quando dois nós são trocados
// na pilha mínima
void minHeapify(MinHeap *minHeap, int idx)
{
    int left, right, smallest;

    left = 2 * idx + 1;
    right = 2 * idx + 2;
    smallest = idx;
    if (left < minHeap->count &&
        minHeap->array[left].frequencia <
            minHeap->array[smallest].frequencia)
        smallest = left;

    if (right < minHeap->count &&
        minHeap->array[right].frequencia <
            minHeap->array[smallest].frequencia)
        smallest = right;

    if (smallest != idx)
    {
        // Atualize o índice correspondente no nó Trie.
        minHeap->array[smallest].root->indexMinHeap = idx;
        minHeap->array[idx].root->indexMinHeap = smallest;

        // Trocar nós na pilha mínima
        swapMinHeapNodes(&minHeap->array[smallest], &minHeap->array[idx]);

        minHeapify(minHeap, smallest);
    }
}

// Uma função padrão para construir um heap
void buildMinHeap(MinHeap *minHeap)
{
    int n, i;
    n = minHeap->count - 1;

    for (i = (n - 1) / 2; i >= 0; --i)
        minHeapify(minHeap, i);
}

// Insere uma palavra no heap, a função lida com os 3 casos explicados acima
void insertInMinHeap(MinHeap *minHeap, arvoreNode **root, const char *word)
{

    // Caso 1: a palavra já está presente em minHeap
    if ((*root)->indexMinHeap != -1)
    {
        ++(minHeap->array[(*root)->indexMinHeap].frequencia);

        // percorrer para baixo
        minHeapify(minHeap, (*root)->indexMinHeap);
    }

    // Caso 2: a Word não está presente e o heap não está cheio

    else if (minHeap->count < minHeap->capacity)
    {
        int count = minHeap->count;
        minHeap->array[count].frequencia = (*root)->frequencia;
        minHeap->array[count].word = (char *)calloc(1, sizeof(char) * strlen(word) + 1);
        strcpy(minHeap->array[count].word, word);

        minHeap->array[count].root = *root;
        (*root)->indexMinHeap = minHeap->count;

        ++(minHeap->count);
        buildMinHeap(minHeap);
    }

    // Caso 3: o Word não está presente e o heap está cheio. E frequência da palavra
    // é mais que root. A raiz é a palavra menos frequente na pilha,
    // substitui root por nova palavra
    else if ((*root)->frequencia > minHeap->array[0].frequencia)
    {

        minHeap->array[0].root->indexMinHeap = -1;
        minHeap->array[0].root = *root;
        minHeap->array[0].root->indexMinHeap = 0;
        minHeap->array[0].frequencia = (*root)->frequencia;

        // exclua a memória alocada anteriormente
        free(minHeap->array[0].word);
        minHeap->array[0].word = (char *)calloc(1, sizeof(char) * strlen(word) + 1);
        strcpy(minHeap->array[0].word, word);

        minHeapify(minHeap, 0);
    }
}

// Insere uma nova palavra para Trie e Heap
void insertUtil(arvoreNode **root, MinHeap *minHeap,
                const char *word, const char *dupWord)
{
    // Caso base
    if (*root == NULL)
        *root = novaArvoreNode();

    // Ainda há mais caracteres na palavra
    if (*word != '\0')
    {
        if ((*word >= 'a' && *word <= 'z') || (*word >= 'A' && *word <= 'Z'))
        { //verifica se caracter valido
            insertUtil(&((*root)->children[tolower(*word) - 97]),
                       minHeap, word + 1, dupWord);
        }
    }

    else // A palavra completa é processada
    {
        // a palavra já está presente, aumente a frequência
        if ((*root)->fim)
            ++((*root)->frequencia);
        else
        {
            (*root)->fim = 1;
            (*root)->frequencia = 1;
        }

        // Insere também no min heap
        insertInMinHeap(minHeap, root, dupWord);
    }
}

// adiciona uma palavra a Trie e min heap. Um invólucro sobre a ferramenta de inserção
void insertTrieAndHeap(const char *word, arvoreNode **root, MinHeap *minHeap)
{
    insertUtil(root, minHeap, word, word);
}

void sort(int n, int *ptr)
{
    int i, j, t;

    // sort os números usando ponteiros
    for (i = 0; i < n; i++)
    {

        for (j = i + 1; j < n; j++)
        {

            if (*(ptr + j) > *(ptr + i))
            {

                t = *(ptr + i);
                *(ptr + i) = *(ptr + j);
                *(ptr + j) = t;
            }
        }
    }
}

// Uma função utilitária para mostrar resultados, A pilha mínima
// contém k palavras mais frequentes até o momento, a qualquer momento
void displayMinHeap(MinHeap *minHeap)
{
    int i;
    int teste_array[minHeap->count];

    //sortear as estructs

    for (i = 0; i < minHeap->count; ++i)
    {
        teste_array[i] = minHeap->array[i].frequencia;
    }

    sort(minHeap->count, teste_array);

    // imprime a palavra K principal com frequência
    for (i = 0; i < minHeap->count; ++i)
    {
        for (int j = 0; j < minHeap->count; j++)
        {
            if (minHeap->array[j].frequencia == teste_array[i])
            {
                printf("%s : %d\n", minHeap->array[j].word,
                       minHeap->array[j].frequencia);
                break;
            }
        }
    }
}

int displayMinHeap2(MinHeap *minHeap)
{
    if (minHeap->array[0].frequencia == 0)
    {
        printf("Palavra não encontrada!!!\n");
    }
    return minHeap->array[0].frequencia;
}

// A principal função que recebe um arquivo como entrada, adicione palavras ao heap
// e Try, finalmente mostra o resultado da pilha
void printResultado(FILE *fp, int k)
{
    // Criar um heap mínimo de tamanho k
    MinHeap *minHeap = createMinHeap(k);

    // Crie um Trie vazio
    arvoreNode *root = NULL;

    // Um buffer para armazenar uma palavra por vez
    char buffer[128];
    int count_palavras = 0;
    int count_tudo = 0;

    // Leia as palavras uma a uma do arquivo. Inserir a palavra em Trie e Min Heap
    while (fscanf(fp, "%s", buffer) != EOF)
    {
        char *cortado = strtok(buffer, "\"-.,*!?°:;''%¨&=+|{}<>~^``()@[]_/$#0123456789 ");
        while (cortado != NULL)
        {
            if ((*cortado >= 'a' && *cortado <= 'z') || (*cortado >= 'A' && *cortado <= 'Z'))
            {
                if (*cortado >= 'A' && *cortado <= 'Z')
                {
                    char *temp = strdup(cortado); // faça uma cópia

                    // ajusta a cópia para minúscula
                    unsigned char *tptr = (unsigned char *)temp;
                    while (*tptr)
                    {
                        *tptr = tolower(*tptr);
                        tptr++;
                    }

                    // faz coisas

                    // libera cópia
                    strcpy(cortado, temp);

                    free(temp);
                }
                insertTrieAndHeap(cortado, &root, minHeap);
                count_palavras++;
            }
            count_tudo++;
            cortado = strtok(NULL, "\"-.,*!?°:;''%¨&=+|{}<>~^``()@[]_/$#0123456789 ");
        }
    }

    // O heap mínimo terá as k palavras mais frequentes, portanto, imprima os nós do heap mínimo
    displayMinHeap(minHeap);

    printf("quantidade de palavras: %d\n", count_palavras);
    printf("quantidade de palavras e simbolos: %d\n", count_tudo);
}

//printResultado modificada para apresentar tf
float printResultado2(FILE *fp, char termo[])
{
    int max = 1;

    MinHeap *minHeap = createMinHeap(max);

    arvoreNode *root = NULL;

    char buffer[128];
    float count_palavras = 0;
    int count_tudo = 0;

    while (fscanf(fp, "%s", buffer) != EOF)
    {
        char *cortado = strtok(buffer, "\"-.,*!?°:;''%¨&=+|<>~{}^``()@[]_/$#0123456789 ");
        while (cortado != NULL)
        {
            if ((*cortado >= 'a' && *cortado <= 'z') || (*cortado >= 'A' && *cortado <= 'Z'))
            {
                if (strcmp(termo, cortado) == 0)
                {
                    char *temp = strdup(cortado);

                    unsigned char *tptr = (unsigned char *)temp;
                    while (*tptr)
                    {
                        *tptr = tolower(*tptr);
                        tptr++;
                    }

                    insertTrieAndHeap(cortado, &root, minHeap);

                    strcpy(cortado, temp);

                    free(temp);
                }
                count_palavras++;
            }
            count_tudo++;

            cortado = strtok(NULL, "\"-.,*!?°:;''%¨&=+|<>~{}^``()@[]_/$#0123456789 ");
        }
    }

    displayMinHeap(minHeap);

    float qtd_Palavra = displayMinHeap2(minHeap);
    //usado para tf
    if (qtd_Palavra > 0)
    {
        float tf = (qtd_Palavra / count_palavras);
        return tf;
    }
    else
    {
        return 0.0;
    }
}

float geraTFIDF(float idf, data *array, int numerodatas)
{

    for (int i = 0; i < numerodatas; i++)
    {
        array[i].tfidf += array[i].tf * idf;
    }

    struct data temp;

    for (int i = 0; i < numerodatas - 1; i++)
    {
        for (int j = i + 1; j < numerodatas; j++)
        {
            if (array[i].tfidf < array[j].tfidf)
            {
                temp = array[i];
                array[i] = array[j];
                array[j] = temp;
            }
        }
    }

    return 1;
}

//teria mais recursos, porém não foi necessario
void somadorTFIDF(float idf, data *array, int numerodatas)
{
    geraTFIDF(idf, array, numerodatas);
}

void geraTF(int numerodatas, char datas[][300], char termo[300], int numeroTermos, data *array)
{
    FILE *fp = NULL;
    float NumerodatasPresente = 0;

    for (int i = 0; i < numerodatas; i++)
    {
        fp = fopen(datas[i], "r");
        if (fp == NULL)
            printf("File doesn't exist ");
        else
        {
            strcpy(array[i].nome, datas[i]);

            array[i].tf = printResultado2(fp, termo);

            if (array[i].tf > 0.0000000)
                NumerodatasPresente++;
        }
    }

    float idf = log(numerodatas / NumerodatasPresente) / log(10);

    somadorTFIDF(idf, array, numerodatas);
}

void freq(char *argv[])
{

    int msec = 0, trigger = 10,iterations; /* 10ms */
    clock_t before = clock();

    char *p;
    int num = strtol(argv[2], &p, 10);

    FILE *fp = fopen(argv[3], "r");
    if (fp == NULL)
        printf("Arquivo nao Encontrado");
    else
    {
        puts("");
        printf("############################################################\n");
        printf("\nExibindo as %d palavras que mais aparecem no arquivo %s\n\n", num, argv[3]);
        printf("############################################################\n");
        printResultado(fp, num);
    }


      clock_t difference = clock() - before;
      msec = difference * 1000 / CLOCKS_PER_SEC;
      iterations++;
      printf("Tempo gasto %d segundos %d millissegundos (%d iteracoes)\n",
      msec/1000, msec%1000, iterations);
}

void freq_word(char *argv[])
{

    int msec = 0, trigger = 10,iterations; /* 10ms */
    clock_t before = clock();

    FILE *fp = fopen(argv[3], "r");
    if (fp == NULL)
        printf("Arquivo nao Encontrado");
    else
    {
        puts("");
        printf("############################################################\n");
        printf("\nA quantidade da palavra =\n\n");
        printResultado2(fp, argv[2]);
    }


      clock_t difference = clock() - before;
      msec = difference * 1000 / CLOCKS_PER_SEC;
      iterations++;
      printf("Tempo gasto %d segundos %d millissegundos (%d iteracoes)\n",
      msec/1000, msec%1000, iterations);
}

void search(char *argv[])
{

    int msec = 0, trigger = 10,iterations; /* 10ms */
    clock_t before = clock();


    char array_arquivos[10][300];
    char array_termos[20][300];
    int n_arqui = 0, n_termo = 0;

    printf("\nOs arquivos mais Relevantes !\n\n");

    char *cortado_arquivo = strtok(argv[3], " ");
    while (cortado_arquivo != NULL)
    {
        if (n_arqui == 10)
        {
            printf("Maximo de 10 arquivos\n");
            break;
        }
        strcpy(array_arquivos[n_arqui], cortado_arquivo);
        n_arqui++;
        cortado_arquivo = strtok(NULL, " ");
    }

    char *cortado_termo = strtok(argv[2], " ");
    while (cortado_termo != NULL)
    {
        if (n_termo == 20)
        {
            printf("Maximo de 20 termos\n");
            break;
        }
        strcpy(array_termos[n_termo], cortado_termo);
        n_termo++;
        cortado_termo = strtok(NULL, " ");
    }

    data *array = arrayDocs(n_arqui);

    for (int w = 0; w < n_termo; w++)
    {
        geraTF(n_arqui, array_arquivos, array_termos[w], n_termo, array);
    }

    for (int q = 0; q < n_arqui; q++)
    {
        printf("arquivo %s   TFIDF: %f\n", array[q].nome, array[q].tfidf);
        iterations++;
    }

      clock_t difference = clock() - before;
      msec = difference * 1000 / CLOCKS_PER_SEC;
      
      printf("Tempo gasto %d segundos %d millissegundos (%d iteracoes)\n",
      msec/1000, msec%1000, iterations);
}

int verificaArgumento(char argumento[])
{

    char *resposta = NULL;

    if (strcmp(argumento, "--freq") == 0)
    {

        return 1;
    }
    else if (strcmp(argumento, "--freq-word") == 0)
    {

        return 2;
    }
    else if (strcmp(argumento, "--search") == 0)
    {

        return 3;
    }

    return 0;
}


