#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct Array {
	int *array;
	int  count;
} Array;

Array array_malloc(int c) {
	Array a;
	a.array = (int *)malloc(sizeof(int) * c);
	a.count = c;
	return a;
}

void array_populate(Array a, const char *el) {
	printf("\nEnter %d %s : ", a.count, el);
	for(int i = 0; i < a.count; i++) scanf("%d", &a.array[i]);
}

void array_print(Array a) {
	printf("{ ");
	for(int i = 0; i < a.count; i++) printf("%d, ", a.array[i]);
	printf("}");
}

void array_populate_random(Array a) {
	srand(time(NULL));
	for(int i = 0; i < a.count; i++) {
		a.array[i] = rand() % 100;
	}
}

int int_compare(const void *a, const void *b) {
	return (*(int *)a - *(int *)b);
}

int quick_sort_partition(int *arr, int left, int right) {
	int pivot = left;
	while(left < right) {
		while(arr[pivot] <= arr[right] && pivot < right) right--;
		if(arr[pivot] > arr[right]) {
			int temp   = arr[pivot];
			arr[pivot] = arr[right];
			arr[right] = temp;
			left++;
			pivot = right;
		}
		while(arr[pivot] >= arr[left] && left < pivot) {
			left++;
		}
		if(arr[pivot] < arr[left]) {
			int temp   = arr[pivot];
			arr[pivot] = arr[left];
			arr[left]  = temp;
			pivot      = left;
			right--;
		}
	}
	return pivot;
}

void quick_sort_rec(Array a, int low, int high) {
	if(low < high) {
		int loc = quick_sort_partition(a.array, low, high);
		if(loc != 0)
			quick_sort_rec(a, low, loc - 1);
		quick_sort_rec(a, loc + 1, high);
	}
}

void quick_sort(Array a) {
	quick_sort_rec(a, 0, a.count - 1);
}

int main() {
	int count = 14;
	printf("Enter number of elements : ");
	scanf("%d", &count);
	Array a = array_malloc(count);
	array_populate(a, "elements");
	// array_populate_random(a);
	printf("\nArray : ");
	array_print(a);
	printf("\nEnter number of passes : ");
	scanf("%d", &count);
	Array increments = array_malloc(count);
	array_populate(increments, "increments");
	int total = a.count;
	for(int i = 0; i < count; i++) {
		int   numLists  = increments.array[i];
		int   increment = increments.array[i];
		Array lists[increment];
		int   el = (total / increment) + ((total % increment) > 0);
		printf("\nIncrement %d : ", increment);
		for(int j = 0; j < increment; j++) {
			lists[j] = array_malloc(el);
			int size = 0;
			for(int k = j; k < total; k += increment) {
				lists[j].array[size++] = a.array[k];
			}
			lists[j].count = size;
			printf("\n\tList %d : ", j);
			array_print(lists[j]);
			quick_sort(lists[j]);
			printf("\n\tList %d after sorting : ", j);
			array_print(lists[j]);
		}
		for(int j = 0; j < increment; j++) {
			for(int k = 0; k < lists[j].count; k++) {
				a.array[j + (k * increment)] = lists[j].array[k];
			}
		}
		printf("\nOriginal array after this pass : ");
		array_print(a);
	}
}
