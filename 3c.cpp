#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
	int vertex;
	struct Node *next;
} Node;

void insertNodeInto(Node **head, Node *toInsert) {
	if(*head == NULL) {
		*head = toInsert;
		return;
	}
	Node *trav = *head;
	while(trav->next != NULL) {
		if(trav->vertex == toInsert->vertex) {
			free(toInsert);	
			return;
		}
		trav = trav->next;
	}
	
	trav->next = toInsert;
}

Node* new_node(int v) {
	Node *n = (Node*)malloc(sizeof(Node));
	n->next = NULL;
	n->vertex = v;
	return n;
}

int main() {
	Node *array[100] = {NULL};
	
	int v, e, a, b, w;
	printf("Enter number of vertices : ");
	scanf("%d", &v);
	if(v >= 100) {
		printf("Number of vertices out of range!\n");
		return 0;
	}
	printf("Enter number of edges : ");
	scanf("%d", &e);
	
	for(int i = 0; i < e;i++) {
		printf("Enter terminal vertices for edge %d : ", i + 1);
		scanf("%d%d", &a, &b);
		if(a > v || b > v || a < 1 || b < 1) {
			printf("Invalid edge (%d, %d)!\n", a, b);
			continue;
		}
		a--;
		b--;
		Node *v2 = new_node(b + 1);
		insertNodeInto(&array[a], v2);
	}
	
	printf("Adjacency list : \n");
	for(int i = 0; i < v; i++){
		printf("V%d ", i + 1);
		Node *head = array[i];
		while(head != NULL) {
			printf("--> V%d ", head->vertex);
			head = head->next;
		}
		printf("--> X\n");
	}
}
