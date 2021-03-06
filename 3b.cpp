#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
	int          vertex;
	int          weight;
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

Node *new_node(int v, int weight) {
	Node *n   = (Node *)malloc(sizeof(Node));
	n->next   = NULL;
	n->vertex = v;
	n->weight = weight;
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

	for(int i = 0; i < e; i++) {
		printf("Enter terminal vertices for edge %d : ", i + 1);
		scanf("%d%d", &a, &b);
		if(a > v || b > v || a < 1 || b < 1) {
			printf("Invalid edge (%d, %d)!\n", a, b);
			continue;
		}
		printf("Enter weight for edge %d : ", i + 1);
		scanf("%d", &w);
		a--;
		b--;
		Node *v1 = new_node(a + 1, w);
		Node *v2 = new_node(b + 1, w);
		insertNodeInto(&array[a], v2);
		insertNodeInto(&array[b], v1);
	}

	printf("Adjacency list : \n");
	for(int i = 0; i < v; i++) {
		printf("V%d ", i + 1);
		Node *head = array[i];
		while(head != NULL) {
			printf("--> V%d(%d) ", head->vertex, head->weight);
			head = head->next;
		}
		printf("--> X\n");
	}
}
