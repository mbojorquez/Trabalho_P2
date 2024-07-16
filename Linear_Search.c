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
} TDicionario;

long long int total_comparisons = 0; // Variável global para contar comparações totais

int TDicionario_Tamanho(TDicionario *D); // Função para obter o tamanho do dicionário
int TDicionario_Tamanho(TDicionario *D) {
    return D->n;
}

TDicionario *TDicionario_Inicia();
TApontador TDicionario_Pesquisa(TDicionario *D, TChave c);
int TDicionario_Insere(TDicionario *D, TItem x);
int TDicionario_Retira(TDicionario *D, TChave c);

TDicionario *TDicionario_Inicia(){
    TDicionario *D;
    D=(TDicionario *) malloc(sizeof(TDicionario));
    D->n=0;
    D->max=10;
    D->Item=(TItem *) malloc(D->max * sizeof(TItem));
    return D;
}

TApontador TDicionario_Pesquisa(TDicionario *D, TChave c){
    TApontador i;
    for (i=0;i<D->n;i++) {
        total_comparisons++; // Incrementa o contador de comparação
        if (D->Item[i].Chave == c)
            return i;
    }
    return NIL;
}

int TDicionario_Insere(TDicionario *D, TItem x){
    total_comparisons++; // Incrementa o contador de comparação
    if (TDicionario_Pesquisa(D, x.Chave) != NIL)
        return 0;
    total_comparisons++; // Incrementa o contador de comparação
    if (D->n == D->max) {
        D->max *= 2;
        D->Item = (TItem *) realloc(D->Item, D->max * sizeof(TItem));
    }
    D->Item[D->n++] = x;
    return 1;
}

int TDicionario_Retira(TDicionario *D, TChave c){
    TApontador i;
    i=TDicionario_Pesquisa(D,c);
    total_comparisons++; // Incrementa o contador de comparação
    if(i==NIL)
        return 0;
    
    D->Item[i]=D->Item[--D->n];
    if (4 * D->n == D->max) {
        D->max /= 2;
        D->Item = (TItem *) realloc(D->Item, D->max * sizeof(TItem));
    }
    return 1;
}

void TDicionario_Imprime(TDicionario *D) {
    printf("Dictionary Contents:\n");
    for (int i = 0; i < D->n; i++) {
        printf("Item %d: Chave = %d\n", i + 1, D->Item[i].Chave);
    }
}

void ArrayToDictionary(int array[], int array_size, TDicionario *dic) {
    for (int i = 0; i < array_size; i++) {
        TItem item = { array[i] }; // Cria TItem com elemento de array como chave
        TDicionario_Insere(dic, item); //Insere item no dicionário
    }
}


int main() {
    clock_t start_t, end_t;
    double total_t;
    int i;
    start_t = clock(); 
    int array_size = sizeof(shared_array) / sizeof(shared_array[0]);

//Sorted
    initialize_array(shared_array, ARRAY_SIZE);
//ALEATORY
    //initialize_array(shared_array, ARRAY_SIZE);    
    //srand((unsigned)time(NULL));
    //shuffle(shared_array, ARRAY_SIZE);
//Nearly Sorted - 0.30% aleatory
    //initialize_array(shared_array, ARRAY_SIZE);
    //srand((unsigned)time(NULL));
    //shuffle_last_portion(shared_array, ARRAY_SIZE, UNSORTED_PERCENT);
//Reverse Sorted
    //initialize_reverse_array(shared_array, ARRAY_SIZE);

    
    printf("Vector inicial: ");
    printim(shared_array, ARRAY_SIZE);  
    // Inicializa um dicionário
    TDicionario *dic = TDicionario_Inicia();

    // Converte array em dicionário
    ArrayToDictionary(shared_array, array_size, dic);

    // INSERCAO (2 ELEMENTOS)
    // Inserir itens no dicionário
    TItem item1 = {ARRAY_SIZE}; 
    TItem item2 = {ARRAY_SIZE+1};
    TDicionario_Insere(dic, item1);
    TDicionario_Insere(dic, item2);
    printf("Items inserted: [%d %d ]", ARRAY_SIZE, ARRAY_SIZE+1);
    int size = TDicionario_Tamanho(dic);
    printf("    | %d elementos\n", size);
   

    int array[]={0,10,1000000};
    int j;
    int array_b_size=0;
    int array_b[3];
    printf("\n>>  Searching for & Delete: [");
    for (int j=0; j<3;j++){
        printf("%d ", array[j]);
    }
    printf("]\n");
    for (j = 0; j < 3; j++) {

    //PESQUISA & REMOCAO (3 ELEMENTOS)
        TChave key_to_search = array[j];
        TApontador result = TDicionario_Pesquisa(dic, key_to_search);
        if (result != NIL) {
            TDicionario_Retira(dic, key_to_search);
            array_b[array_b_size]=key_to_search;
            array_b_size++;
        } else {
            printf("Item com chave %d não encontrado.\n", key_to_search);
        }
    }
    
    printf("Items encontrados e removidos:  [");
    for (int k=0; k<array_b_size;k++){
        printf("%d ", array_b[k]);
    }
    printf("]");


    int f_size = TDicionario_Tamanho(dic);
    printf("\nTotal de elementos: %d\n", f_size);

    //Finish Time
    end_t = clock();   
    total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;
    printf("\nTotal de comparações: %lld", total_comparisons);    
    printf("\nTempo: %f\n", total_t  );
    
    free(dic->Item);
    free(dic);

    return 0;
}
