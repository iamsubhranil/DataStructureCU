#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct Array {
    int *values;
    int size;
} Array;

Array arr_new(int size) {
    Array a;
    a.values = (int *)malloc(sizeof(int) * size);
    a.size = size;
    return a;
}

void arr_print(Array a) {
    for(int i = 0; i < a.size; i++) {
        printf("%d ", a.values[i]);
    }
    printf("\n");
}


void arr_populate_rand(Array a, int limit) {
    srand(time(NULL));
    for(int i = 0; i < a.size; i++) {
        a.values[i] = rand() % limit;
    }
}

#define lchild(x)   ((x*2) + 1)
#define rchild(x)   ((x * 2) + 2)

#define swap(x, y)  { \
    int temp = x; \
    x = y; \
    y = temp; \
}

Array make_heap(Array input) {
    Array heap = arr_new(input.size);
    for(int i = 0;i < input.size;i++){
        heap.values[i] = input.values[i];
        int j = i;
        while(j > 0){
            int parent = (j+1)/2 - 1;
            // Min heap
            if(heap.values[j] < heap.values[parent]){
                swap(heap.values[j], heap.values[parent]);
                j = parent;
                continue;
            }
            break;
        }
    }
    return heap;
}

Array heap_sort(Array a) {
    Array ret = arr_new(a.size);
    Array heap = make_heap(a);
    printf("Heap : ");
    arr_print(heap);
    for(int i = 0; i < a.size; i++) {
        ret.values[i] = heap.values[0];
        heap.values[0] = heap.values[heap.size - 1];
        heap.size--;
        if(heap.size > 0)
            heap = make_heap(heap);
    }
    return ret;
}

int main() {
    Array a = arr_new(10);
    arr_populate_rand(a, 100);
    printf("Input array : ");
    arr_print(a);
    printf("\n");
    Array ret = heap_sort(a);
    printf("After sort : ");
    arr_print(ret);
    printf("\n");
}