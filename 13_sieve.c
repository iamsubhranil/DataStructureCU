#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ARRX(name, sig, type)    \
typedef struct name {  \
    type *arr; \
    int size; \
} name; \
\
name sig##_new(int size) { \
    name a; \
    if(size == 0) \
        a.arr = NULL; \
    else \
        a.arr = (type *)malloc(sizeof(type) * size); \
    a.size = size; \
    return a; \
} \
\
void sig##_insert(name *a, type val) { \
    a->arr = (type*)realloc(a->arr, sizeof(type)*++a->size); \
    a->arr[a->size - 1] = val; \
}

ARRX(Array, arr, int)
ARRX(Array2, arr2, Array)

void arr_print(Array a) {
    printf("{ ");
    if (a.size > 0) {
        printf("%d", a.arr[0]);
        for(int i = 1; i < a.size; i++) {
            printf(", %d", a.arr[i]);
        }
    }
    printf("}");
}

void arr_populate_random(Array a, int max) {
    for(int i = 0;i < a.size;i++) {
        a.arr[i] = rand() % max;
    }
}

void arr_reverse(Array a) {
	//arr_print(a);
	//printf("\n");
	for(int i = 0, j = a.size - 1;i < a.size/2;i++,j--) {
		int temp = a.arr[i];
		a.arr[i] = a.arr[j];
		a.arr[j] = temp;
	}
	//arr_print(a);
	//printf("\n");
}

Array sieve_sort(Array elements) {
    if(elements.size == 1)
        return elements;
    Array2 arrays = arr2_new(0);
    Array res = arr_new(0);
    int picked[elements.size];
    for(int i = 0;i < elements.size;i++)
    	picked[i] = 0;
    int remaining = 1;
    while(remaining) {
        int first = 0;
        while(first < elements.size && picked[first])
            first++;
        if(first < elements.size) {
            Array part = arr_new(0);
            int item = elements.arr[first++];
            picked[first - 1] = 1;
            arr_insert(&part, item);
            while(first < elements.size) {
                if(item > elements.arr[first] && !picked[first]) {
                    arr_insert(&part, elements.arr[first]);
                    picked[first] = 1;
                }
                first++;
            }
            if(part.size > 0) {
            	arr_reverse(part);
                arr2_insert(&arrays, part);
            }
        } else {
            remaining = 0;
        }
    }
    for(int i = 0;i <  arrays.size;i++) {
        Array temp = sieve_sort(arrays.arr[i]);
        for(int j = 0; j < temp.size;j++) {
            arr_insert(&res, temp.arr[j]);
        }
    }
    return res;
}

int check_sort(Array a) {
    for(int i = 0;i < a.size - 1;i++) {
        if(a.arr[i] > a.arr[i + 1])
            return 0;
    }
    return 1;
}

int main() {
    srand(time(NULL));
    Array a = arr_new(10000);
    arr_populate_random(a, 100);
    //printf("\nOriginal array : ");
    //arr_print(a);
    Array res = sieve_sort(a);
    //printf("\nAfter sort     : ");
    //arr_print(res);
    if(check_sort(res)) {
        printf("\nSorting successful!\n");
    }
}
