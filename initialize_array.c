//#include "shared_array.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


//static int shared_array[ARRAY_SIZE];

void initialize_array(int *array, size_t size) {
    for (size_t i = 0; i < size; i++) {
        array[i] = (int)i;
    }
}

void shuffle(int *array, size_t n) {
    if (n > 1) {
        for (size_t i = 0; i < n - 1; i++) {
            size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
            int t = array[i];
            array[i] = array[j];
            array[j] = t;
        }
    }
}

// Function to shuffle the last portion of the array
void shuffle_last_portion(int *array, size_t size, double percent) {
    size_t start_index = (size_t)(size * (1 - percent));
    size_t num_elements_to_shuffle = size - start_index;
    
    for (size_t i = start_index; i < size; i++) {
        size_t j = start_index + rand() % num_elements_to_shuffle;
        
        // Swap elements at indices i and j
        int temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }
}

void initialize_reverse_array(int *array, size_t size) {
    for (size_t i = 0; i < size; i++) {
        array[i] = (int)(size - 1 - i);
    }
}

void printim(int *array, size_t size) {
//    printf("\n");
//    printf("Existen %zu elementos en el array: ", size);
    printf("[");    
    for (size_t i = 0; i < 3; i++) {
        printf("%d ", array[i]);
    }
    printf("... ");
    for (size_t i = size-3; i < size; i++) {
        printf("%d ", array[i]);
    }
    printf("]");
//    printf("La posicion %zu es: %d", size-1, array[size-1]);
    printf("    | %lu elementos", size);
    printf("\n");
}

void printArray(int *array, int size){
    int i;
    for (int i=0;i<size;i++)
        printf("%d ", array[i]);
    printf("\n");
//    printf("n es: %i\n", n);   
}
