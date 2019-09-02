#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Graph {
	int numOfVertices;
	int numOfEdges;
	int **adjacency_matrix;
} Graph;

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
		g->adjacency_matrix[b][a] = 1;
	}
}

// returns -1 if not found
int findNextAdjacent(int **adjacency_matrix, int *visited, int numVertex, int vertex) {
	for(int i = 0; i < numVertex;i++) {
		if(adjacency_matrix[vertex][i] == 1 && visited[i] == 0)
			return i;
	}
	return -1;
}

void dfs(int **adjacency_matrix, int *visited, int numVertex, int vertex) {
	int next;
	printf(" %d -> ", vertex + 1);
	visited[vertex] = 1;
	while((next = findNextAdjacent(adjacency_matrix, visited, numVertex, vertex)) != -1) {
		dfs(adjacency_matrix, visited, numVertex, next);
	}
}

int main() {
	Graph g = {0, 0, NULL};
	inputGraph(&g);
	int start;
	printf("Enter starting vertex : ");
	scanf("%d", &start);
	int *visited = (int*)malloc(sizeof(int) * g.numOfVertices);
	memset(visited, 0, sizeof(int) * g.numOfVertices);
//	printf("%d ", start);
	dfs(g.adjacency_matrix, visited, g.numOfVertices, start - 1);
}
