#include <stdio.h>
#include <stdlib.h>
#define NIL -1
#include "shared_array.h"
#include "initialize_array.c"

typedef int TChave;
typedef struct {
    TChave Chave;
    /* data */
} TItem;

typedef int TApontador;

typedef struct {
    TItem *Item;
    int n, max;
    long long int total_comparisons; // Contador para comparações totais

} TDicionario;

int TDicionario_Tamanho(TDicionario *D); // Função para obter o tamanho do dicionário
int TDicionario_Tamanho(TDicionario *D) {
    return D->n;
}

TDicionario *TDicionario_Inicia(){
    TDicionario *D;
    D=(TDicionario *) malloc(sizeof(TDicionario));
    D->n=0;
    D->max=10;
    D->Item=(TItem *) malloc(D->max * sizeof(TItem));
    D->total_comparisons = 0; // Inicializar contador de comparações totais

    return D;
}

//enfoque pesquisa recursiva
//TApontador TDicionario_Binaria(TDicionario *D, TApontador esq, TApontador dir, TChave c){
//    TApontador meio;
//    meio =(esq+dir)/2;
//    if(esq>dir)
//        return NIL;
//    else if ( c > D->Item[meio].Chave)
//        return TDicionario_Binaria(D,meio+1,dir,c);
//    else if ( c < D->Item[meio].Chave)
//        return TDicionario_Binaria(D, esq, meio-1,c);
//    else
//        return meio;
//    return TDicionario_Binaria(D,0,D->n-1,c);
//}

//Pesquisa iterativa
TApontador TDicionario_Pesquisa(TDicionario *D, TChave c, int *comparisons) {
    TApontador meio, esq, dir;
    *comparisons = 0; // Inicializar contador de comparação 
    esq = 0;
    dir = D->n - 1;
    while (esq <= dir) {
        meio = (esq + dir) / 2;
        (*comparisons)++;
        if (c > D->Item[meio].Chave) {
            esq = meio + 1;
        } else if (c < D->Item[meio].Chave) {
            dir = meio - 1;
        } else {
            return meio;
        }
    }

    return NIL;
}

//Insercao
int TDicionario_Insere(TDicionario *D, TItem x, int *comparisons) {
    TApontador i;
    *comparisons = 0; // Inicializar contador de comparação 
    if (TDicionario_Pesquisa(D, x.Chave, comparisons) != NIL)
        return 0;

    if (D->n == D->max) {
        D->max *= 2;
        D->Item = (TItem *) realloc(D->Item, D->max * sizeof(TItem));
    }
    i = D->n - 1;
    while ((i >= 0) && (x.Chave < D->Item[i].Chave)) {
        D->Item[i + 1] = D->Item[i];
        i--;
        (*comparisons)++;
    }
    D->Item[i + 1] = x;
    D->n++;
    D->total_comparisons += *comparisons; // Atualizar comparações totais
    return 1;
}

//Remove usando pesquisa iterativa
int TDicionario_Retira(TDicionario *D, TChave c, int *comparisons) {
    TApontador i;
    *comparisons = 0; // Inicializar contador de comparação local
    i = TDicionario_Pesquisa(D, c, comparisons);
    if (i == NIL)
        return 0;
    while (i < D->n - 1) {
        D->Item[i] = D->Item[i + 1];
        i++;
    }
    D->n--;

    if (4 * D->n == D->max) {
        D->max /= 2;
        D->Item = (TItem *) realloc(D->Item, D->max * sizeof(TItem));
    }
    D->total_comparisons += *comparisons; // Atualizar comparações totais
    return 1;
}

void TDicionario_Imprime(TDicionario *D) {
    printf("Dictionary Contents:\n");
    for (int i = 0; i < D->n; i++) {
        printf("Item %d: Chave = %d\n", i + 1, D->Item[i].Chave);
    }
}

void PrintDictionaryAsArray(TDicionario *D) {
    printf("[");
    for (int i = 0; i < D->n; i++) {
        printf("%d", D->Item[i].Chave);
        if (i < D->n - 1) {
            printf(", ");
        }
    }
    printf("]");
}

void ArrayToDictionary(int array[], int array_size, TDicionario *dic) {
    for (int i = 0; i < array_size; i++) {
        TItem item = { array[i] }; // Cria TItem com elemento de vetor como chave
        int comparisons;
        TDicionario_Insere(dic, item, &comparisons); // Inserir item no dicionário
    }
}

int TDicionario_Size(TDicionario *D) {
    return D->n;
}

int main() {
    clock_t start_t, end_t;
    double total_t;
    int i;
    start_t = clock(); 
    int array_size = sizeof(shared_array) / sizeof(shared_array[0]);

//Sorted
    //initialize_array(shared_array, ARRAY_SIZE);
//ALEATORY
    //initialize_array(shared_array, ARRAY_SIZE);    
    //srand((unsigned)time(NULL));
    //shuffle(shared_array, ARRAY_SIZE);
//Nearly Sorted - 0.30% aleatory
    //initialize_array(shared_array, ARRAY_SIZE);
    //srand((unsigned)time(NULL));
    //shuffle_last_portion(shared_array, ARRAY_SIZE, UNSORTED_PERCENT);
//Reverse Sorted
    initialize_reverse_array(shared_array, ARRAY_SIZE);

    
    // Inicializar um dicionário
    TDicionario *dic = TDicionario_Inicia();
    // Converter vetor em dicionário
    ArrayToDictionary(shared_array, array_size, dic);

    //INSERCAO (2 ELEMENTOS)
    TItem item1 = {ARRAY_SIZE}; 
    TItem item2 = {ARRAY_SIZE+1};
    int comparisons;
    TDicionario_Insere(dic, item1, &comparisons);
    TDicionario_Insere(dic, item2, &comparisons);
    printf("Items inserted: [%d %d]", ARRAY_SIZE, ARRAY_SIZE+1 );

    int size = TDicionario_Tamanho(dic);
    printf("\nTotal de elementos %d\n", size);
   

    int array[]={0,10,1000000};
    int j;
    int array_b_size=0;
    int array_b[3];
    printf("\n>>  Searching for & deleting: [");
    for (int j=0; j<3;j++){
        printf("%d ", array[j]);
    }
    printf("]\n");
    for (j = 0; j < 3; j++) {

    //PESQUISA & REMOCAO
        TChave key_to_search = array[j];
        TApontador result = TDicionario_Pesquisa(dic, key_to_search, &comparisons);
        if (result != NIL) {
            TDicionario_Retira(dic, key_to_search,&comparisons);
            array_b[array_b_size]=key_to_search;
            array_b_size++;
        } else {
            printf("Item com chave %d não encontrado.\n", key_to_search);
        }
    }
    
    printf("Items encontrados e removidos: [");
    for (int k=0; k<array_b_size;k++){
        printf("%d ", array_b[k]);
    }
    printf("]");

    int f_size = TDicionario_Tamanho(dic);
    printf("\nTotal de elementos: %d\n", f_size);
    //Finish Time
    end_t = clock();   
    total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;
    printf("\nTotal de comparações: %lld", dic->total_comparisons);
    printf("\nTempo %f\n", total_t  );
    free(dic->Item);
    free(dic);

    return 0;
}
