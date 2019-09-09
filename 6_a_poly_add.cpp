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
		p.terms[i] = {0, 0};
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

Polynomial poly_add(Polynomial p1, Polynomial p2) {
	// first, calculate the resulting number of terms
	// after addition
	int num = 0, i = 0, j = 0;
	while(i < p1.no && j < p2.no) {
			num++;
		if(p1.terms[i].exp == p2.terms[j].exp) {
			i++;
			j++;
		} else if(p1.terms[i].exp > p2.terms[j].exp) {
			i++;
		} else {
			j++;
		}
	}
	if(i < p1.no) {
		num += (p1.no - i);
	} else {
		num += (p2.no - j);
	}
	// allocate exact amount of memory
	Polynomial res = poly_new(num);
	i = 0; j = 0;
	int k = 0;
	while(i < p1.no && j < p2.no) {
		if(p1.terms[i].exp == p2.terms[j].exp) {
			res.terms[k].exp = p1.terms[i].exp;
			res.terms[k].coeff = p1.terms[i].coeff + p2.terms[j].coeff;
			i++;
			j++;
		} else if(p1.terms[i].exp > p2.terms[j].exp) {
			res.terms[k] = p1.terms[i];
			i++;
		} else {
			res.terms[k] = p2.terms[i];
			j++;
		}
		k++;
	}
	while(i < p1.no) {
		res.terms[k++] = p1.terms[i++];
	}
	while(j < p2.no) {
		res.terms[k++] = p2.terms[j++];
	}
	return res;
}

void poly_print_matched(int min, int max, Polynomial p1) {
	for(int i = max, j = 0;i >= min && j < p1.no;i--) {
		if(i != max) {
			printf(" + ");
		}
		int coeff = 0;
		if(i == p1.terms[j].exp) {
			coeff = p1.terms[j].coeff;
			j++;
		}
		printf("%2dx%-2d", coeff, i);
	}
}

void poly_print_tabular(Polynomial p1, Polynomial p2, Polynomial p3, const char *s) {
	// find the maximum and minimum exponents
	int max = p1.terms[0].exp > p2.terms[0].exp ? 
				p1.terms[0].exp : p2.terms[0].exp;
	int min = p1.terms[p1.no - 1].exp < p2.terms[p2.no - 1].exp ? 
				p1.terms[p1.no - 1].exp : p2.terms[p2.no - 1].exp;
				
	int indent = 2 + strlen(s) + 1;
	printf("\n%*s", indent, " ");
	poly_print_matched(min, max, p1);
	printf("\n  %s ", s);
	poly_print_matched(min, max, p2);
	printf("\n");
	int line = indent + 7*(max - min) + 10;
	while(line--)
		printf("-");
	printf("\n");
	printf("%*s", indent, " ");
	poly_print_matched(min, max, p3);
	
}

int main() {
	Polynomial p1 = poly_input(" first ");
	printf("\nFirst polynomial is : ");
	poly_print(p1);
	printf("\n");
	Polynomial p2 = poly_input(" second ");
	printf("\nSecond polynomial is : ");
	poly_print(p2);
	printf("\n");
	Polynomial p3 = poly_add(p1, p2);
	printf("\nSum : ");
	poly_print_tabular(p1, p2, p3, " + ");
}
