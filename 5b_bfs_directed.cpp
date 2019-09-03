#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Graph {
	int numOfVertices;
	int numOfEdges;
	int **adjacency_matrix;
} Graph;

typedef struct Queue {
	int *elements;
	int front;
	int rear;
	int max;
	int size;
} Queue;

void queue_new(Queue* q, int size) {
	q->elements = (int*)malloc(sizeof(int) * size);
	q->max = size;
	q->front = 0;
	q->rear = 0;
	q->size = 0;
}

int queue_isEmpty(Queue *q) {
	 return q->size == 0;
}

int queue_delete(Queue *q) {
	if(q->size == 0) {
		printf("\nQueue underflow!");
		return -1;
	}
	int ret = q->elements[q->front];
	q->front = (q->front + 1) % q->max;
	q->size--;
	return ret;
}

void queue_insert(Queue *q, int val) {
	if(q->size == q->max) {
		printf("\nQueue overflow!");
		return;
	}
	q->elements[q->rear] = val;
	q->rear = (q->rear + 1) % q->max;
	q->size++;
}

void inputGraph(Graph *g) {
	printf("Enter number of vertices : ");
	scanf("%d", &g->numOfVertices);
	printf("Enter the number of edges : ");
	scanf("%d", &g->numOfEdges);
	g->adjacency_matrix = (int **)malloc(sizeof(int*) * g->numOfVertices);
	for(int i = 0; i < g->numOfVertices; i++) {
		g->adjacency_matrix[i] = (int*)malloc(sizeof(int) * g->numOfVertices);
		memset(g->adjacency_matrix[i], 0, sizeof(int) * g->numOfVertices);
	}
	int v = g->numOfVertices;
	for(int i = 0; i < g->numOfEdges; i++) {
		printf("Enter the terminal vertices for edge %d : ", i + 1);
		int a, b;
		scanf("%d%d", &a, &b);
		if(a > v || b > v || a < 1 || b < 1){
			printf("Invalid edge : (%s, %s)\n", a, b);
			i--;
			continue;
		}
		g->adjacency_matrix[--a][--b] = 1;
	//	g->adjacency_matrix[b][a] = 1;
	}
}

void readGraph(Graph *g, const char *file) {
	FILE *f = fopen(file, "r");
	if(f == NULL) {
		printf("Error: Unable to read '%s'!\n", file);
		return;
	}
	fscanf(f, "%d", &g->numOfVertices);
	fscanf(f, "%d", &g->numOfEdges);
	g->adjacency_matrix = (int **)malloc(sizeof(int*) * g->numOfVertices);
	for(int i = 0; i < g->numOfVertices; i++) {
		g->adjacency_matrix[i] = (int*)malloc(sizeof(int) * g->numOfVertices);
		memset(g->adjacency_matrix[i], 0, sizeof(int) * g->numOfVertices);
	}
	for(int i = 0; i < g->numOfEdges; i++) {
		int a, b;
		fscanf(f, "%d%d", &a, &b);
		g->adjacency_matrix[--a][--b] = 1;
	//	g->adjacency_matrix[b][a] = 1;
	}
	fclose(f);
}

// returns -1 if not found
int findNextAdjacent(int **adjacency_matrix, int *visited, int numVertex, int vertex) {
	for(int i = 0; i < numVertex;i++) {
		if(adjacency_matrix[vertex][i] == 1 && visited[i] == 0)
			return i;
	}
	return -1;
}

void bfs(int **adjacency_matrix, int *visited, int numVertex, int start) {
	Queue q;
	queue_new(&q, numVertex);
	queue_insert(&q, start);
	while(!queue_isEmpty(&q)) {
		int vertex = queue_delete(&q);
		visited[vertex] = 1;
		int next;
		while((next = findNextAdjacent(adjacency_matrix, visited, numVertex, vertex)) != -1) {
			visited[next] = 1;
			queue_insert(&q, next);
		}
		printf("%d -> ", vertex + 1);
	}
}

int main() {
	Graph g = {0, 0, NULL};
//	inputGraph(&g);
	readGraph(&g, "graphinput.txt");
	int start;
	printf("Enter starting vertex : ");
	scanf("%d", &start);
	int *visited = (int*)malloc(sizeof(int) * g.numOfVertices);
	memset(visited, 0, sizeof(int) * g.numOfVertices);
	printf("Component 1 : ");
	bfs(g.adjacency_matrix, visited, g.numOfVertices, start - 1);
	int comp = 2;
	for(int i = 0; i < g.numOfVertices; i++) {
		if(visited[i] == 0) {
			printf("\nComponent %d : ", comp++);
			bfs(g.adjacency_matrix, visited, g.numOfVertices, i);
		}
	}
}
