#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	int coeff, exp;
} Term;

typedef struct {
	Term *terms;
	int no;
} Polynomial;

Polynomial poly_new(int n) {
	Polynomial p = {NULL, n};
	p.terms = (Term *)malloc(sizeof(Term) * n);
	for(int i = 0; i < n; i++) {
		p.terms[i] = {0};
	}
	return p;
}

Polynomial poly_input(const char *s) {
	Polynomial p = {NULL, 0};
	printf("Number of terms in the%spolynomial : ", s);
	scanf("%d", &p.no);
	p.terms = (Term *)malloc(sizeof(Term) * p.no);
	for(int i = 0;i < p.no;i++) {
		printf("\nEnter coefficient and exponent of term %d : ", i + 1);
		scanf("%d%d", &(p.terms[i].coeff), &(p.terms[i].exp));
	}
	// sort by the exponent in descending order
	for(int i = 0; i < p.no; i++) {
		int changed = 0;
		for(int j = 0; j < p.no - i - 1; j++) {
			if(p.terms[j].exp < p.terms[j + 1].exp) {
				Term t = p.terms[j];
				p.terms[j] = p.terms[j + 1];
				p.terms[j + 1] = t;
				changed = 1;
			}
		}
		if(!changed)
			break;
	}
	return p;
}

void poly_print(Polynomial p) {
	printf("%dx%d", p.terms[0].coeff, p.terms[0].exp);
	for(int i = 1; i < p.no;i++) {
		printf(" + %dx%d", p.terms[i].coeff, p.terms[i].exp);
	}
}

void poly_diff(Polynomial *p) {
	for(int i = 0; i < p->no; i++) {
		p->terms[i].exp = p->terms[i].exp - 1;
		p->terms[i].coeff = p->terms[i].coeff * (p->terms[i].exp + 1);
	}
}

int main() {
	Polynomial p1 = poly_input(" ");
	printf("\nThe polynomial is : ");
	poly_print(p1);
	printf("\n");
	poly_diff(&p1);
	printf("\nDifferentiated polynomial : ");
	poly_print(p1);
}
