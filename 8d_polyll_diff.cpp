
#include <stdio.h>
#include <stdlib.h>

typedef struct PolyNode {
	int       coeff, exp;
	PolyNode *next;
} PolyNode;

PolyNode *poly_create(int coeff, int exp) {
	PolyNode *node = (PolyNode *)malloc(sizeof(PolyNode));
	node->coeff    = coeff;
	node->exp      = exp;
	node->next     = NULL;
	return node;
}

void poly_insert_into(PolyNode **head, PolyNode *next) {
	while(*head != NULL) {
		if((*head)->exp < next->exp) {
			next->next = (*head);
			*head      = next;
			return;
		} else if((*head)->exp == next->exp) {
			(*head)->coeff += next->coeff;
			free(next);
			return;
		}
		head = &((*head)->next);
	}
	*head = next;
}

void poly_print(PolyNode *p) {
	PolyNode *head = p;
	if(head != NULL) {
		printf("%2dx%-2d ", head->coeff, head->exp);
		head = head->next;
	}
	while(head != NULL) {
		printf("+ %2dx%-2d ", head->coeff, head->exp);
		head = head->next;
	}
}

PolyNode *poly_input(bool fromFile, const char *fileName) {
	FILE *f;
	if(fromFile) {
		f = fopen(fileName, "r");
		if(f == NULL) {
			printf("[Error] Unable to open file '%s'!", fileName);
			exit(1);
		}
	} else {
		f = stdin;
	}
	int       no, coeff, exp;
	PolyNode *head = NULL;
	if(!fromFile)
		printf("Enter number of terms : ");
	fscanf(f, "%d", &no);
	for(int i = 0; i < no; i++) {
		if(!fromFile)
			printf("Enter coefficient and exponent of term %d : ", i + 1);
		fscanf(f, "%d%d", &coeff, &exp);
		poly_insert_into(&head, poly_create(coeff, exp));
	}
	if(fromFile)
		fclose(f);
	return head;
}

PolyNode *poly_diff(PolyNode *p1) {
	PolyNode *ret = NULL;
	while(p1 != NULL) {
		int coeff = p1->coeff;
		int exp   = p1->exp;
		if(exp != 0) {
			coeff *= exp;
			exp -= 1;
			poly_insert_into(&ret, poly_create(coeff, exp));
		}
		p1 = p1->next;
	}
	return ret;
}

int main() {
	PolyNode *p1 = poly_input(true, "poly1.txt");
	PolyNode *d  = poly_diff(p1);
	printf("Given polynomial : ");
	poly_print(p1);
	printf("\n  Differentiated : ");
	poly_print(d);
}
