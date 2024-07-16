#include <stdio.h>
#include <stdlib.h>
#include "shared_array.h"
#include "initialize_array.c"

typedef int TChave;
typedef struct {
    TChave Chave;
    /* data */
} TItem;

typedef struct SNo *TArvBin;

typedef struct SNo {
    TItem Item;
    TArvBin Esq, Dir;
} TNo;

#define NIL NULL

typedef TArvBin TApontador;

typedef struct {
    TArvBin Raiz;
    int n;
}   TDicionario;

int total_comparisons = 0; // Variável global para contar comparações totais

TDicionario *TDicionario_Inicia(){
    TDicionario *D;
    D=(TDicionario *) malloc(sizeof(TDicionario));
    D->n=0;
    D->Raiz=NULL;
    return D;
}

//Pesquisa Recursiva
TArvBin Pesquisa(TArvBin No, TChave c) {
    if (No == NULL)
        return NULL;
    else {
        total_comparisons++; // Incremento para cada comparação feita
        if (c < No->Item.Chave)
            return Pesquisa(No->Esq, c);
        else if (c > No->Item.Chave)
            return Pesquisa(No->Dir, c);
        else
            return No;
    }
}

//Pesquisa Iterativa
//TArvBin Pesquisa_1(TArvBin Raiz, TChave c){
//    TArvBin No;
//    No=Raiz;
//    while ((No!=NULL) && (c!=No->Item.Chave)){
//        if (c<No->Item.Chave)
//            No=No->Esq;
//        else if (c>No->Item.Chave)
//            No=No->Dir;
//    }
//    return No;
//}

//Insercao Recursiva
int Insere(TArvBin *pNo, TItem x) {
    if (*pNo == NULL) {
        *pNo = (TArvBin) malloc(sizeof(TNo));
        (*pNo)->Item = x;
        (*pNo)->Esq = NULL;
        (*pNo)->Dir = NULL;
        return 1;
    } else {
        total_comparisons++; // Incremento para cada comparação feita
        if (x.Chave < (*pNo)->Item.Chave)
            return Insere(&(*pNo)->Esq, x);
        else if (x.Chave > (*pNo)->Item.Chave)
            return Insere(&(*pNo)->Dir, x);
        else
            return 0;
    }
}

void Sucessor(TArvBin *q, TArvBin *r){
    if ((*r)->Esq !=NULL)
        Sucessor(q, &(*r)->Esq);
    else {
        (*q)->Item =(*r)->Item;
        *q=*r;
        *r=(*r)->Dir;
    }
}

//Remocao Recursiva
int Retira(TArvBin *p, TChave c) {
    TArvBin q;
    if (*p == NULL)
        return 0;
    else {
        total_comparisons++; // Incremento para cada comparação feita
        if (c < (*p)->Item.Chave)
            return Retira(&(*p)->Esq, c);
        else if (c > (*p)->Item.Chave)
            return Retira(&(*p)->Dir, c);
        else {
            q = *p;
            if (q->Esq == NULL)
                *p = q->Dir;
            else if (q->Dir == NULL)
                *p = q->Esq;
            else
                Sucessor(&q, &q->Dir);
            free(q);
            return 1;
        }
    }
}

void Predecessor (TArvBin *q, TArvBin *r){
    if ((*r)->Dir != NULL)
        Predecessor(q,&(*r)->Dir);
    else {
        (*q)->Item=(*r)->Item;
        *q=*r;
        *r=(*r)->Esq;
    }
}


TApontador TDicionario_Pesquisa(TDicionario *D, TChave c){
    return Pesquisa(D->Raiz, c);
}

int TDicionario_Insere(TDicionario *D, TItem x){
    if (!Insere(&D->Raiz,x))
        return 0;
    D->n++;
    return 1;
}

int TDicionario_Retira(TDicionario *D, TChave c){
    if (!Retira(&D->Raiz,c))
        return 0;
    D->n--;
    return 1;
}

// Função para imprimir o conteúdo do dicionário (percurso em ordem)
void PrintDictionary(TArvBin No) {
    if (No != NULL) {
        PrintDictionary(No->Esq);
        printf("%d ", No->Item.Chave); // Print the key
        PrintDictionary(No->Dir);
    }
}

// Função para imprimir o conteúdo do dicionário
void TDicionario_Print(TDicionario *D) {
    printf("Dictionary content (in-order traversal): ");
    PrintDictionary(D->Raiz);
    printf("\n");
}


// Função para converter um array de TItem em um dicionário
TDicionario *ArrayToDictionary(TItem *array, int size) {
    TDicionario *dict = TDicionario_Inicia();
    int i;
    for (i = 0; i < size; i++) {
        TDicionario_Insere(dict, array[i]);
    }
    return dict;
}
// Função recursiva para contar nós na árvore
int ContaNos(TArvBin No) {
    if (No == NULL)
        return 0;
    else
        return 1 + ContaNos(No->Esq) + ContaNos(No->Dir);
}

int ContaNosDictionary(TDicionario *D) {
    return ContaNos(D->Raiz);
}

// Função para converter um array de inteiros em um array de TItem
TItem *IntegersToTItems(int *array, int size) {
    TItem *items = (TItem *) malloc(size * sizeof(TItem));
    for (int i = 0; i < size; i++) {
        items[i].Chave = array[i];
    }
    return items;
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

    TDicionario *dic = TDicionario_Inicia();
    TItem *shared_array_1 = IntegersToTItems(shared_array, array_size);
    TDicionario *dict = ArrayToDictionary(shared_array_1, array_size);

    //INSERCAO (2 ELEMENTOS)
    TItem item1 = { ARRAY_SIZE }; // Example item with key 5
    TItem item2 = { ARRAY_SIZE+1 };
    TDicionario_Insere(dict, item1);
    TDicionario_Insere(dict, item2);
    printf("Inserted:\n");
    printf("[%d %d ]\n", (ARRAY_SIZE), (ARRAY_SIZE+1));

    TChave keys_to_search[] = { 0, 10, 1000000 };
    int num_keys = sizeof(keys_to_search) / sizeof(keys_to_search[0]);

    //PESQUISA & REMOCAO (3 ELEMENTOS)
    printf("\nSearching & deleting for elements in dictionary:\n");
    for (int i = 0; i < num_keys; i++) {
        TArvBin result = TDicionario_Pesquisa(dict, keys_to_search[i]);
        if (result != NULL) {
            TDicionario_Retira(dict, keys_to_search[i]);
            printf("Chave %d encontrada & removida\n", keys_to_search[i]);
        } else {
            printf("Chave %d nao encontrada\n", keys_to_search[i]);
        }
    }

    printf("\nNúmero de nós na árvore:  %d\n", ContaNosDictionary(dict));

    //TDicionario_Print(dict);
    //Finish Time
    end_t = clock();   
    total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;
    printf("Total de comparações: %d\n", total_comparisons);
    printf("Tempo: %f\n", total_t  );
    free(dict); // Free allocated memory for the dictionary
    return 0;
}