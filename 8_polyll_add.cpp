
#include <stdio.h>
#include <stdlib.h>

typedef struct PolyNode {
	int coeff, exp;
	PolyNode *next;
} PolyNode;

PolyNode* poly_create(int coeff, int exp) {
	PolyNode *node = (PolyNode*)malloc(sizeof(PolyNode));
	node->coeff = coeff;
	node->exp = exp;
	node->next = NULL;
	return node;
}

void poly_insert_into(PolyNode **head, PolyNode *next) {
	while(*head != NULL) {
		if((*head)->exp < next->exp) {
			next->next = (*head);
			*head = next;
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

void poly_print(PolyNode* p) {
	PolyNode *head = p;
	if(head  != NULL) {
		printf("%2dx%-2d ", head->coeff, head->exp);
		head = head->next;
	}
	while(head != NULL) {
		printf("+ %2dx%-2d ", head->coeff, head->exp);
		head = head->next;
	}
}

PolyNode* poly_input(bool fromFile, const char *fileName) {
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
	int no, coeff, exp;
	PolyNode *head = NULL;
	if(!fromFile)
		printf("Enter number of terms : ");
	fscanf(f, "%d", &no);
	for(int i = 0;i < no;i++){
		if(!fromFile)
			printf("Enter coefficient and exponent of term %d : ", i + 1);
		fscanf(f, "%d%d", &coeff, &exp);
		poly_insert_into(&head, poly_create(coeff, exp));
	}
	if(fromFile)
		fclose(f);
	return head;
}

PolyNode* poly_add(PolyNode *p1, PolyNode *p2) {
	PolyNode *head = NULL;
	while(p1 != NULL && p2 != NULL) {
		int coeff, exp;
		if(p1->exp == p2->exp) {
			coeff = p1->coeff + p2->coeff;
			exp = p1->exp;
			p1 = p1->next;
			p2 = p2->next;
		} else if(p1->exp > p2->exp) {
			coeff = p1->coeff;
			exp = p1->exp;
			p1 = p1->next;
		} else {
			coeff = p2->coeff;
			exp = p2->exp;
			p2 = p2->next;
		}
		poly_insert_into(&head, poly_create(coeff, exp));
	}
	while(p1 != NULL) {
		poly_insert_into(&head, poly_create(p1->coeff, p1->exp));
		p1 = p1->next;
	}
	while(p2 != NULL) {
		poly_insert_into(&head, poly_create(p2->coeff, p2->exp));
		p2 = p2->next;
	}
	return head;
}

void poly_print_upto(int max, int min, PolyNode *p2) {
	int first = 0;
	for(int i = max;i >= min && p2 != NULL;i--) {
		int coeff = 0;
		if(p2->exp == i) {
			coeff = p2->coeff;
			p2 = p2->next;
		}
		if(first) {
			printf(" + ");
		} else
			first = 1;
		printf("%2dx%-2d", coeff, i);
	}
}

void poly_print_aligned(PolyNode *p1, PolyNode *p2, PolyNode *res, const char sign) {
	PolyNode *tp1 = p1, *tp2 = p2;
	int max = p1->exp > p2->exp ? p1->exp : p2->exp;
	while(tp1->next != NULL) tp1 = tp1->next;
	while(tp2->next != NULL) tp2 = tp2->next;
	int min = tp1->exp < tp2->exp ? tp1->exp : tp2->exp;
	printf("\n    ");
	poly_print_upto(max, min, p1);
	printf("\n  %c ", sign);
	poly_print_upto(max, min, p2);
	printf("\n-----------------------------------------\n    ");
	poly_print_upto(max, min, res);
}

int main() {
	PolyNode* p1 = poly_input(true, "poly1.txt");
	PolyNode* p2 = poly_input(true, "poly2.txt");
	PolyNode *sum = poly_add(p1, p2);
	poly_print_aligned(p1, p2, sum, '+');
}
