#include <stdio.h>
#include <stdlib.h>
#include <time.h>
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
    int fb; //fator de balanceamiento -1, 0 ou +1
} TNo;

// Variável global para contar comparações totais
long long int total_comparisons = 0;

// Incrementa a função do contador de comparação
void increment_comparison_counter() {
    total_comparisons++;
}

//O(n)
int Altura(TArvBin No){
    int hEsq, hDir;
    if (No ==NULL)
        return -1; //altura de arvore vazia e -1
    hEsq=Altura(No->Esq);
    hDir=Altura(No->Dir);
    if (hEsq > hDir)
        return hEsq+1;
    else
        return hDir+1;
}
//O(n), agora O(1)
int FB(TArvBin No){
    if (No==NULL)
        return 0;
    //return Altura (No->Esq)-Altura(No->Dir);
    return No->fb;
}

int ArvoreAVL(TArvBin No){
    int fb;
    if (No==NULL)
        return 1;

    if (!ArvoreAVL(No->Esq))
        return 0;
    if (!ArvoreAVL(No->Dir))
        return 0;
    
    fb=FB(No);
    if ((fb>1) || (fb <-1))
        return 0;
    else
        return 1;
}

void LL(TArvBin *pA){
    TArvBin pB;
    pB=(*pA)->Esq;
    (*pA)->Esq=pB->Dir;
    pB->Dir=*pA;
    if (pB->fb==0) {(*pA)->fb=+1;pB->fb=-1;}
    else           {(*pA)->fb=0;pB->fb=0;}
    *pA=pB;
}

void RR(TArvBin *pA){
    TArvBin pB;
    pB=(*pA)->Dir;
    (*pA)->Dir=pB->Esq;
    pB->Esq=*pA;
    if (pB->fb==0) { (*pA)->fb=-1; pB->fb=+1;}
    else           { (*pA)->fb=0; pB->fb=0;}
    *pA=pB;
}

void LR(TArvBin *pA){
    TArvBin pB, pC;
    pB=(*pA)->Esq;
    pC=pB->Dir;
    pB->Dir=pC->Esq;
    pC->Esq=pB;
    (*pA)->Esq=pC->Dir;
    pC->Dir=*pA;
    if (pC->fb==+1) (*pA)->fb=-1; else (*pA)->fb=0;
    if (pC->fb==-1) pB->fb=+1; else pB->fb=0;
    pC->fb=0;
    *pA=pC;
}

void RL(TArvBin *pA){
    TArvBin pB, pC;
    pB=(*pA)->Dir;
    pC=pB->Esq;
    pB->Esq=pC->Dir;
    pC->Dir=pB;
    (*pA)->Dir=pC->Esq;
    pC->Esq=*pA;
    if (pC->fb==-1) (*pA)->fb=+1; else (*pA)->fb=0;
    if (pC->fb==+1) pB->fb=-1; else pB->fb=0;
    pC->fb=0;
    *pA=pC;
}

int BalancaEsquerda(TArvBin *pNo){
    int fbe =FB((*pNo)->Esq);
    if (fbe>0){
        LL(pNo);
        return 1;
    }
    else if (fbe<0){
        LR(pNo);
        return 1;
    }
    else {
        LL(pNo);
        return 0;
    }
}

int BalancaDireita(TArvBin *pNo){
    int fbd =FB((*pNo)->Dir);
    if (fbd<0){
        RR(pNo);
        return 1;
    }
    else if (fbd>0){
        RL(pNo);
        return 1;
    }
    else {
        RR(pNo);
        return 0;
    }
}

int BalancaNo(TArvBin *pNo){
    int fb=FB(*pNo);
    if (fb>1)
        return BalancaEsquerda(pNo);
    else if (fb<-1)
        return BalancaDireita(pNo);
    return 0;
}

//Pesquisa Recursiva
TArvBin Pesquisa(TArvBin No, TChave c){
    if (No==NULL)
        return NULL;
    else if (c < No->Item.Chave) {
        increment_comparison_counter(); // Increment comparison counter
        return Pesquisa(No->Esq,c);
    }
    else if (c > No->Item.Chave) {
        increment_comparison_counter(); // Increment comparison counter
        return Pesquisa(No->Dir,c);
    }
    else {
        increment_comparison_counter(); // Increment comparison counter
        return No;
    }
}

//INSERCAO: Custo, O(h)
int Insere(TArvBin *pNo, TItem x){
    if (*pNo==NULL){
        *pNo=(TArvBin) malloc(sizeof(TNo));
        (*pNo)->Item=x;
        (*pNo)->Esq=NULL;
        (*pNo)->Dir=NULL;
        (*pNo)->fb=0;
        return 1;
    }
    else if (x.Chave<(*pNo)->Item.Chave){
        increment_comparison_counter(); // Increment comparison counter
        if(Insere(&(*pNo)->Esq, x))
            switch((*pNo)->fb){ //subarvore esquerda cresceu
                case -1: (*pNo)->fb=0; return 0;
                case 0: (*pNo)->fb=+1; return 1;
                case +1: return !BalancaEsquerda(pNo);
            }
        return 0;
    }
    else if (x.Chave>(*pNo)->Item.Chave){
        increment_comparison_counter(); // Increment comparison counter
        if (Insere(&(*pNo)->Dir, x))
            switch((*pNo)->fb){ //subarvore direita cresceu
                case +1: (*pNo)->fb=0; return 0;
                case 0: (*pNo)->fb=-1; return 1;
                case -1: return !BalancaDireita(pNo);
            }
            return 0;
    }
    else
        return 0;// retorna 0 caso o item ja estiver na arvore
}

int Sucessor(TArvBin *q, TArvBin *r){
    if ((*r)->Esq !=NULL){
        if (Sucessor(q, &(*r)->Esq))
            switch ((*r)->fb){
                case +1: (*r)->fb=0; return 1;
                case 0: (*r)->fb=-1; return 0;
                case -1: return BalancaDireita(r);
            }
        return 0;
    }
    else {
        (*q)->Item =(*r)->Item;
        *q=*r;
        *r=(*r)->Dir;
        return 1;
    }
}

//REMOCAO, Custo O(h)
int Retira(TArvBin *p, TChave c){
    TArvBin q; int ret;
    if (*p==NULL)
        return 0; // retorna 0 caso o item nao esteja na arvore
    else if (c<(*p)->Item.Chave){
        increment_comparison_counter(); // Increment comparison counter
        if(Retira(&(*p)->Esq, c))
            switch((*p)->fb){ //subarvore esquerda cresceu
                case +1: (*p)->fb=0; return 1;
                case 0: (*p)->fb=-1; return 0;
                case -1: return BalancaDireita(p);
            }
        return 0;
    }
    else if (c>(*p)->Item.Chave){
        increment_comparison_counter(); // Increment comparison counter
        if (Retira(&(*p)->Dir, c))
            switch((*p)->fb){ //subarvore direita cresceu
                case -1: (*p)->fb=0; return 1;
                case 0: (*p)->fb=+1; return 0;
                case +1: return BalancaEsquerda(p);
            }
        return 0;
    }
    else {
        q=*p;
        if (q->Esq==NULL ) { *p=q->Dir; ret=1;}
        else if (q->Dir==NULL) {*p=q->Esq; ret=1;}
        else {//possui dois filhos
            if (Sucessor(&q,&q->Dir))
                switch ((*p)->fb){
                    case -1: (*p)->fb=0; ret=1; break;
                    case 0: (*p)->fb=+1; ret=0; break;
                    case +1: ret=BalancaEsquerda(p); break;
                }
            else ret=0;
        }
        free(q);
        return ret;
    }
}

//Função para imprimir a árvore AVL (percurso em-ordem)
void PrintAVL(TArvBin No) {
    if (No != NULL) {
        PrintAVL(No->Esq);
        printf("%d ", No->Item.Chave);
        PrintAVL(No->Dir);
    }
}

//Função para converter um array inteiro em um array de estruturas TItem
TItem* ArrayToIntItem(int array[], int size) {
    TItem* items = (TItem*) malloc(size * sizeof(TItem));
    if (items == NULL) {
        fprintf(stderr, "Erro ao alocar memória para array de TItem.\n");
        exit(EXIT_FAILURE);
    }
    
    for (int i = 0; i < size; i++) {
        items[i].Chave = array[i];
    }
    
    return items;
}

int ContaNos(TArvBin No) {
    if (No == NULL)
        return 0;
    else
        return ContaNos(No->Esq) + ContaNos(No->Dir) + 1;
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

    // Inicializa a árvore AVL como NULL
    TArvBin AVL = NULL;
    // Converte o array em um array de estruturas TItem
    TItem* items = ArrayToIntItem(shared_array, array_size);    
    //Insere elementos na árvore AVL
    for (int i = 0; i < array_size; i++) {
        Insere(&AVL, items[i]);
    }

    TItem item1 = { ARRAY_SIZE }; // Example item with key 5
    TItem item2 = { ARRAY_SIZE+1 };

    //INSERCAO (2 ELEMENTOS)
    Insere(&AVL, item1);
    Insere(&AVL, item2);

    printf("\n>>  Inserted:\n");
    printf("[%d %d ]\n", (ARRAY_SIZE), (ARRAY_SIZE+1) );
    
    int searchKeys[] = {0, 10, 1000000}; // Chaves para pesquisar e excluir
    int keysToDelete[] = {0, 10, 1000000}; // Keys to delete
    int numSearches = sizeof(searchKeys) / sizeof(int);
    int numDeletes = sizeof(keysToDelete) / sizeof(int);
    
    
    //PESQUISA & REMOCAO (3 ELEMENTOS)
    printf("\n>>  Searching & Deleting:\n");
    for (int i = 0; i < numDeletes; i++) {
        TChave chave = keysToDelete[i];
        TArvBin resultado = Pesquisa(AVL, chave);
        if (resultado != NULL) {
            Retira(&AVL, chave);
            printf("Número %d encontrada e retirada na árvore\n", keysToDelete[i]);
        } else {
            printf("Número %d nao encontrada na árvore\n", keysToDelete[i]);
        }
    }
    // Print AVL tree after deletions
    //printf("AVL Tree after deletion (inorder traversal):\n");
    //PrintAVL(AVL);
    printf("\n");
    int numNos = ContaNos(AVL);
    printf("Número de nós na árvore AVL: %d\n", numNos);
    printf("Total de comparações: %lld\n", total_comparisons);    
    //Finish Time
    end_t = clock();   
    total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;
    printf("Tempo: %f\n", total_t  );

    // Free allocated memory for items array
    free(items);
    
    return 0;
}