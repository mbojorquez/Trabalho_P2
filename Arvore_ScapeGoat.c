#define UNSORTED_PERCENT 0.30
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "shared_array.h"
#include "initialize_array.c"

//Define uma estrutura para um nó na árvore
typedef struct Node {
    struct Node* left;
    struct Node* right;
    struct Node* parent;
    float value;
} Node;

// Define uma estrutura para a árvore ScapeGoat
typedef struct SGTree {
    Node* root;
    int n; // Número de nós
    int q; // Fator de tamanho do limite superior
    int counter; // Contador para comparações totais
} SGTree;

// Protótipos de funções
void inorder(Node* node);
int size(Node* node);
int insert(SGTree* sgt, float x);
Node* search(Node* node, float x);
Node* findMin(Node* node);
int delete(SGTree* sgt, float x);
void rebuildTree(SGTree* sgt, Node* u);
Node* buildBalancedFromArray(Node** a, int i, int n);
int BSTInsertAndFindDepth(SGTree* sgt, Node* u);
int storeInArray(Node* u, Node** a, int i);
int depth(Node* node);

// Percorre a árvore em ordem e imprime os valores dos nós
void inorder(Node* node) {
    if (node != NULL) {
        inorder(node->left);
        printf("%.1f ", node->value);
        inorder(node->right);
    }
}

// Obtém o tamanho da subárvore enraizada em um determinado nó
int size(Node* node) {
    if (node == NULL)
        return 0;
    return 1 + size(node->left) + size(node->right);
}

// Calcula a profundidade de um nó
int depth(Node* node) {
    int d = 0;
    while (node != NULL) {
        node = node->parent;
        d++;
    }
    return d - 1;
}

//Insere um novo nó na árvore
int insert(SGTree* sgt, float x) {
    Node* node = (Node*)malloc(sizeof(Node));
    if (node == NULL)
        return 0; // Falha na alocação de memória

    node->value = x;
    node->left = NULL;
    node->right = NULL;
    node->parent = NULL;

    //Insere o nó na árvore e obtém sua profundidade
    int h = BSTInsertAndFindDepth(sgt, node);

    // Verifica se a árvore está desbalanceada
    if (h > ceil(2.0 * log(sgt->n + 1) / log(3.0))) {
        Node* p = node->parent;

        //Encontre a raiz da subárvore a ser reconstruída
        while (p != NULL && 3 * size(p) <= 2 * size(p->parent)) {
            p = p->parent;
        }
        rebuildTree(sgt, p); //Reconstrui a subárvore
    }

    return h >= 0;
}

// Pesquisa um nó com um valor específico
Node* search(Node* node, float x) {
    if (node == NULL || node->value == x)
        return node;

    if (x < node->value)
        return search(node->left, x);
    else
        return search(node->right, x);
}

//Encontre o nó mínimo em uma subárvore
Node* findMin(Node* node) {
    if (node == NULL)
        return NULL;

    while (node->left != NULL)
        node = node->left;

    return node;
}

//Exclui um nó da árvore
int delete(SGTree* sgt, float x) {
    Node* node = search(sgt->root, x);
    if (node == NULL)
        return 0; // Nó não encontrado

    Node* parent = node->parent;

    // Caso 1: o nó não tem filhos (nó folha)
    if (node->left == NULL && node->right == NULL) {
        if (parent == NULL)
            sgt->root = NULL;
        else if (parent->left == node)
            parent->left = NULL;
        else
            parent->right = NULL;
        free(node);
    }
    // Caso 2: o nó tem um filho
    else if (node->left == NULL || node->right == NULL) {
        Node* child = (node->left != NULL) ? node->left : node->right;
        if (parent == NULL)
            sgt->root = child;
        else if (parent->left == node)
            parent->left = child;
        else
            parent->right = child;
        free(node);
    }
    // Caso 3: o nó tem dois filhos
    else {
        Node* successor = findMin(node->right);
        node->value = successor->value;
        delete(sgt, successor->value);
    }

    sgt->n--;
    return 1;
}

//Função para reconstruir a árvore do novo nó. Esta função basicamente usa storeInArray()
//para primeiro armazenar a precurso em-ordem do BST enraizada em u em um array.
//Em seguida, ele converte o array para o BST mais balanceado possível usando buildBalancedFromArray()
void rebuildTree(SGTree* sgt, Node* u) {
    if (u == NULL)
        return;

    int n = size(u);
    Node* p = u->parent;
    Node** a = (Node**)malloc(n * sizeof(Node*));
    if (a == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }

    storeInArray(u, a, 0); // Chamando storeInArray

    if (p == NULL) {
        sgt->root = buildBalancedFromArray(a, 0, n);
        if (sgt->root != NULL)
            sgt->root->parent = NULL;
    } else if (p->right == u) {
        p->right = buildBalancedFromArray(a, 0, n);
        if (p->right != NULL)
            p->right->parent = p;
    } else {
        p->left = buildBalancedFromArray(a, 0, n);
        if (p->left != NULL)
            p->left->parent = p;
    }

    free(a);
}

// Função para armazenar precurso em-ordem do BST enraizada em u no array a começando no índice i
int storeInArray(Node* u, Node** a, int i) {
    if (u == NULL)
        return i;

    i = storeInArray(u->left, a, i);
    a[i++] = u;
    return storeInArray(u->right, a, i);
}

//Função para construir uma árvore com nós balanceados a partir de um array
Node* buildBalancedFromArray(Node** a, int i, int n) {
    if (n <= 0)
        return NULL;

    int m = n / 2;
    a[i + m]->left = buildBalancedFromArray(a, i, m);
    if (a[i + m]->left != NULL)
        a[i + m]->left->parent = a[i + m];

    a[i + m]->right = buildBalancedFromArray(a, i + m + 1, n - m - 1);
    if (a[i + m]->right != NULL)
        a[i + m]->right->parent = a[i + m];

    return a[i + m];
}

// Executa a inserção BST padrão e retorna a profundidade do nó inserido.
int BSTInsertAndFindDepth(SGTree* sgt, Node* u) {
    Node* w = sgt->root;
    if (w == NULL) {
        sgt->root = u;
        sgt->n++;
        return 0;
    }

    int done = 0;
    int d = 0;

    while (!done) {
        sgt->counter++;
        if (u->value < w->value) {
            if (w->left == NULL) {
                w->left = u;
                u->parent = w;
                done = 1;
            } else {
                w = w->left;
            }
        } else if (u->value > w->value) {
            if (w->right == NULL) {
                w->right = u;
                u->parent = w;
                done = 1;
            } else {
                w = w->right;
            }
        } else {
            return -1;
        }
        d++;
    }

    sgt->n++;
    return d;
}

//Função para converter um array para estrutura de árvore ScapeGoat
void convertArrayToScapegoatTree(SGTree* sgt, float arr[], int n) {
    for (int i = 0; i < n; i++) {
        insert(sgt, arr[i]);
    }
}

//Função para converter um array int em um array float
void convertIntArrayToFloatArray(int intArray[], float floatArray[], int size) {
    for (int i = 0; i < size; i++) {
        floatArray[i] = (float)intArray[i]; // Convert int to float and assign
    }
}


int main() {
    clock_t start_t, end_t;
    double total_t;
    start_t = clock();

    SGTree sgt;
    sgt.root = NULL;
    sgt.n = 0;
    sgt.counter = 0; // Inicializar contador de comparações


    //Determina o tamanho do shared_array
    int array_size = sizeof(shared_array) / sizeof(shared_array[0]);

    //sorted
    initialize_array(shared_array, array_size);
    
    //Reverse
    //initialize_reverse_array(shared_array, array_size);
    
    //Random
    //initialize_array(shared_array, ARRAY_SIZE);    
    //srand((unsigned)time(NULL));
    //shuffle(shared_array, ARRAY_SIZE);
    
    //Nearly Sorted - 0.30% aleatory
    //initialize_array(shared_array, array_size);
    //srand((unsigned)time(NULL));
    //shuffle_last_portion(shared_array, array_size, UNSORTED_PERCENT);


    // Converte array int em array float
    float shared_array_1[array_size];
    convertIntArrayToFloatArray(shared_array, shared_array_1, array_size);

    // Converte array em Scapegoat Tree structure
    convertArrayToScapegoatTree(&sgt, shared_array_1, array_size);

    //INSERÇÃO
    // Inserindo elementos adicionais na árvore
    insert(&sgt, (float)(array_size));
    insert(&sgt, (float)(array_size + 1));

    // PESQUISA & REMOÇÃO
    float search_values[] = {0,10,1000000};
    for (int i = 0; i < 3; i++) {
        Node* result = search(sgt.root, search_values[i]);
        if (result != NULL) {
            printf("Found %.1f in the tree.\n", search_values[i]);
            delete(&sgt, search_values[i]);
        } else {
            printf("%.1f is not found in the tree.\n", search_values[i]);
        }
    }

    // Finish Time
    end_t = clock();
    total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;
    printf("Total comparações: %d\n", sgt.counter);
    printf("Tempo: %f seconds\n", total_t);

    //Imprimindo a travessia em ordem da árvore ScapeGoat construída
    //printf("Precurso em-ordem após inserção: ");
    //inorder(sgt.root);
    //printf("\n");

    return 0;
}
