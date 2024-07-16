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
    int cor; //fator de balanceamiento -1, 0 ou +1
} TNo;

// Variável global para contar comparações
long long int total_comparisons = 0; 

// Função para aumentar o contador de comparação
void increment_comparison_counter() {
    total_comparisons++;
}

int EhNegro(TArvBin No){
    return (No==NULL) || (No->cor==0);
}

int EhRubro(TArvBin No){
    return (No != NULL) && (No->cor==1);
}

//Custo: O(n)
int AlturaNegra (TArvBin No){
    int hEsq, hDir;
    if(No==NULL)
        return 0; // altura negra de arvore vazia e 0
    hEsq=AlturaNegra(No->Esq);
    hDir=AlturaNegra(No->Dir);
    if (hEsq>hDir)
        return hEsq+EhNegro(No);
    else
        return hDir+EhNegro(No);
}

//Custo: O(n)
int ArvoreARN(TArvBin No){
    if (No==NULL)
        return 1;
    if (!ArvoreARN(No->Esq))
        return 0;
    if (!ArvoreARN(No->Dir))
        return 0;
    if (EhRubro(No) && (!EhNegro(No->Esq) || !EhNegro(No->Dir)))
        return 0;
    if (AlturaNegra(No->Esq) != AlturaNegra(No->Dir))
        return 0;
    return 1;
}

void InverteCor(TArvBin No){
    if (No!=NULL) No->cor = !No->cor;
}

void TrocaCores(TArvBin No){
    InverteCor(No);
    InverteCor(No->Esq);
    InverteCor(No->Dir);
}

void RotacaoDireita(TArvBin *pA){
    TArvBin pB;
    pB=(*pA)->Esq;
    (*pA)->Esq=pB->Dir;
    pB->Dir=*pA;
    *pA=pB;
}

void RotacaoEsquerda(TArvBin *pA){
    TArvBin pB;
    pB=(*pA)->Dir;
    (*pA)->Dir=pB->Esq;
    pB->Esq=*pA;
    *pA=pB;
}

//Custo: O(1)
int BalancaDireita(TArvBin *pC){
    TArvBin pD=(*pC)->Dir;
    if (EhRubro(pD)){ //caso 4a
        RotacaoEsquerda(pC);
        pC=&(*pC)->Esq;
        pD=(*pC)->Dir;
    }
    if (pD!=NULL){
        if(EhNegro(pD->Esq) && EhNegro(pD->Dir)){ //caso 1
            InverteCor(pD);
            if (EhRubro(*pC)){
                InverteCor(*pC);
                return 0;
            }
        }
        else {
            int cor =(*pC)->cor;
            (*pC)->cor=0;
            if (EhNegro(pD->Dir))   //caso 2a
                RotacaoDireita(&(*pC)->Dir);
            else
                InverteCor(pD->Dir);   //caso 3a
            RotacaoEsquerda(pC);
            (*pC)->cor=cor;
            return 0;
        }
    }
    return 1;
}

int BalancaEsquerda(TArvBin *pC){
    TArvBin pD=(*pC)->Esq;
    if (EhRubro(pD)){ //caso 4a
        RotacaoDireita(pC);
        pC=&(*pC)->Dir;
        pD=(*pC)->Esq;
    }
    if (pD!=NULL){
        if(EhNegro(pD->Esq) && EhNegro(pD->Dir)){ //caso 1
            InverteCor(pD);
            if (EhRubro(*pC)){
                InverteCor(*pC);
                return 0;
            }
        }
        else {
            int cor =(*pC)->cor;
            (*pC)->cor=0;
            if (EhNegro(pD->Esq))   //caso 2a
                RotacaoEsquerda(&(*pC)->Esq);
            else
                InverteCor(pD->Esq);   //caso 3a
            RotacaoDireita(pC);
            (*pC)->cor=cor;
            return 0;
        }
    }
    return 1;
}

//Custo: O(1)
void BalancaNo (TArvBin *pA, TArvBin *pB, TArvBin *pC){
    if ((pC!=NULL) && EhRubro(*pA) && EhRubro(*pB)){
        if (*pA == (*pC)->Esq)
            BalancaEsquerda(pC);
        else
            BalancaDireita(pC);
    }
}

//Pesquisa Recursiva
TArvBin Pesquisa(TArvBin No, TChave c){
    if (No==NULL)
        return NULL;
    else if (c < No->Item.Chave) {
        increment_comparison_counter(); // Increment comparison counter
        return Pesquisa(No->Esq,c);
    }
    else if (c > No->Item.Chave){
        increment_comparison_counter(); // Increment comparison counter
        return Pesquisa(No->Dir,c);
    }
    else {
        increment_comparison_counter(); // Increment comparison counter
        return No;
    }
}

//Custo: O(h)
void InsereRecursivo(TArvBin *pA, TArvBin *pC, TItem x){
    if (*pA==NULL){
        *pA=(TArvBin) malloc(sizeof(TNo));
        (*pA)->Item=x; (*pA)->Esq=NULL; (*pA)->Dir=NULL;
        (*pA)->cor=1; //o novo no e rubro
    }
    else if (x.Chave < (*pA)->Item.Chave){
        increment_comparison_counter(); // Increment comparison counter        
        InsereRecursivo(&(*pA)->Esq, pA, x);
        BalancaNo(pA, &(*pA)->Esq, pC);
    }
    else if (x.Chave > (*pA)->Item.Chave){
        increment_comparison_counter(); // Increment comparison counter
        InsereRecursivo(&(*pA)->Dir, pA, x);
        BalancaNo(pA, &(*pA)->Dir, pC);
    }
}

//Custo: O(h)
void Insere(TArvBin *pRaiz, TItem x){
    InsereRecursivo(pRaiz, NULL, x);
    (*pRaiz)->cor=0; // a raiz e negra
}

int Sucessor(TArvBin *q, TArvBin *r){
    int bh;
    if ((*r)->Esq != NULL){
        if (Sucessor(q, &(*r)->Esq))
            return BalancaDireita(r);
        return 0;
    }
    else {
        (*q)->Item=(*r)->Item;
        *q=*r;
        *r=(*r)->Dir;
        bh=EhNegro(*q) && EhNegro (*r);
        if (!bh && EhNegro(*q))
            (*r)->cor=0;
        return bh;
    }
} 

//REMOCAO
int RetiraRecursivo(TArvBin *p, TChave x){
    TArvBin q;
    int bh;
    if (*p == NULL)
        return 0; // retorna 0 caso o item nao esteja na arvore
    else if (x < (*p)->Item.Chave){
        increment_comparison_counter(); // Increment comparison counter
        if (RetiraRecursivo(&(*p)->Esq, x)){
            return BalancaDireita(p); //subarv esquerda encolheu
        }
        return 0;
    }
    else if (x > (*p)->Item.Chave){
        increment_comparison_counter(); // Increment comparison counter
        if (RetiraRecursivo(&(*p)->Dir, x)){
            return BalancaEsquerda(p); //subarv direita encolheu
        }
        return 0;
    }
    else {
        q = *p;
        if (q->Esq == NULL){
            *p = q->Dir;
        }
        else if (q->Dir == NULL){
            *p = q->Esq;
        }
        else {
            if (Sucessor(&q, &q->Dir)){
                bh = BalancaEsquerda(p);
            }
            else {
                bh = 0;
            }
        }
        bh = EhNegro(*p) && EhNegro(q);
        if (!bh && EhNegro(q)){
            (*p)->cor = 0;
        }
        free(q);
        return bh;
    }
}

void Retira(TArvBin *pRaiz, TChave x){
    RetiraRecursivo(pRaiz, x);
    if (*pRaiz != NULL)
        (*pRaiz)->cor=0; // a raiz e negra
}


//Função para imprimir o conteúdo da árvore usando precurso em-ordem
void PrintTreeInOrder(TArvBin No) {
    if (No != NULL) {
        PrintTreeInOrder(No->Esq);
        printf("%d ", No->Item.Chave);
        PrintTreeInOrder(No->Dir);
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

    TArvBin ARN = NULL;

    // Converte o array em um array de estruturas TItem
    TItem* items = ArrayToIntItem(shared_array, array_size);
    
    //Insira elementos na árvore ARN
    for (int i = 0; i < array_size; i++) {
        Insere(&ARN, items[i]);
    }

    TItem items_i[] = {{ARRAY_SIZE},{ARRAY_SIZE+1}};
    int num_items = sizeof(items_i) / sizeof(items_i[0]);

    //INSERÇÃO (2 ELEMENTOS)
    for (int i = 0; i < num_items; ++i) {
        Insere(&ARN, items_i[i]);
        printf("Inserted %d: ", items_i[i].Chave);
    }


    int searchKeys[] = {0, 10, 1000000}; // Chaves para pesquisar e excluir
    int numSearches = sizeof(searchKeys) / sizeof(int);
    
    // PESQUISA & REMOÇÃO (3 ELEMENTOS)    
    printf("\n>>  Searching & Deleting:\n");
    for (int i = 0; i < numSearches; i++) {
        TChave chave = searchKeys[i];
        TArvBin resultado = Pesquisa(ARN, chave);
        if (resultado != NULL) {
            Retira(&ARN, chave);
            printf("Número %d encontrada e retirada na árvore.\n", chave);
        } else {
            printf("Número %d não encontrada na árvore.\n", chave);
        }
    }

    printf("\n");
    int numNos = ContaNos(ARN);
    printf("Número de nós na árvore AVL: %d\n", numNos);

    //Finish Time
    end_t = clock();   
    total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;
    printf("Total comparações: %lld\n", total_comparisons); // Print total comparisons
    printf("Tempo: %f\n", total_t  );
    //PrintTreeInOrder(ARN);
    // Free allocated memory for items array
    free(items);

    return 0;
}
