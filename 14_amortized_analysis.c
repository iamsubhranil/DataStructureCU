#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Sequence {
	bool * s;
	size_t size;
} Sequence;

typedef struct Cost {
	size_t t, c, a;
} Cost;

Sequence seq_new(size_t size) {
	Sequence s;
	s.s = (bool *)malloc(sizeof(bool) * (size + 1));
	memset(s.s, 0, sizeof(bool) * size);
	s.size = size;
	return s;
}

void seq_free(Sequence s) {
	free(s.s);
}

void seq_print(Sequence s) {
	size_t i = s.size - 1;
	for(; i >= 0; i--) {
		printf("%c", '0' + s.s[i]);
		if(i == 0)
			break;
	}
}

Cost seq_next(Sequence s, Cost old) {
	Cost   newc = {0, old.c, 0};
	size_t i    = 0;
	while(!(s.s[i] = !s.s[i])) {
		newc.c--;
		newc.t++;
		i++;
	}
	if(i < s.size) {
		newc.c++;
		newc.t++;
	}
	newc.a = newc.t + (newc.c - old.c);
	return newc;
}

void print_header() {
	printf("Step\tSequence\tTi\tCi\tAi\n");
	printf("====\t========\t==\t==\t==\n");
}

void print_row(size_t step, Sequence s, Cost c) {
	printf("%4zu\t", step);
	size_t i = s.size;
	while(i++ < 8) printf(" ");
	seq_print(s);
	if(c.t == 0) {
		printf("\t--\t%2zu\t--", c.c);
	} else {
		printf("\t%2zu\t%2zu\t%2zu", c.t, c.c, c.a);
	}
	printf("\n");
}

int main() {
	size_t size = 50;
	// for(size_t i = 1; i <= size;i++)
	//    printf("%zu,", i);
	// printf("\n");
	// for(size_t k = 31; k <= size; k++) {
	printf("Enter number of bits : ");
	scanf("%zu", &size);
	printf("Bits : %zu\n", size);
	Sequence s = seq_new(size);
	print_header();
	Cost   orig       = {1, 0, 0};
	size_t max        = 1 << s.size;
	size_t total_cost = 0;
	print_row(0, s, orig);
	for(size_t i = 0; i < max; i++) {
		orig = seq_next(s, orig);
		print_row(i + 1, s, orig);
		total_cost += orig.t;
	}
	seq_free(s);
	// printf("%zu,", total_cost);
	printf("Total cost for %3zu bits : %zu\n", size, total_cost);
	//}
}
