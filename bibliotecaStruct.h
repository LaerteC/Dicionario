
// =================================
//   BIBLIOTECA Structs arvoreNode, MinhHeapNode, MinHeapNode !!!
// =================================


#define MAX_CARACTER 26
#define MAX_TAMANHO_PALAVRA 40



typedef struct arvoreNode arvoreNode;
struct arvoreNode
{
    bool fim;                           
    unsigned frequencia;                
    int indexMinHeap;                   
    arvoreNode *children[MAX_CARACTER]; 
};


struct MinHeapNode
{
    arvoreNode *root;    
    unsigned frequencia; 
    char *word;          
};

typedef struct MinHeapNode MinHeapNode;


typedef struct MinHeap MinHeap;
struct MinHeap
{
    unsigned capacity; 
    int count;          
    MinHeapNode *array; 
};


typedef struct data data;
struct data
{
    char nome[100];
    double tf;
    double tfidf;
};