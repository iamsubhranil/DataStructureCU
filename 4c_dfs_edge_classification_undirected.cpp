#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Graph {
	int numOfVertices;
	int numOfEdges;
	int **adjacency_matrix;
} Graph;

typedef struct Marker {
	int dfs = 0, dfsc = 0;
} Marker;

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
		g->adjacency_matrix[a][b] = 1;
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
		g->adjacency_matrix[b][a] = 1;
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

void dfs(int **adjacency_matrix, int *visited, int numVertex, int vertex, Marker* markers) {
	int next;
	static int DFSN = 1, DFSC = 1;
	printf(" %d -> ", vertex + 1);
	visited[vertex] = 1;
	markers[vertex].dfs = DFSN++;
	while((next = findNextAdjacent(adjacency_matrix, visited, numVertex, vertex)) != -1) {
		adjacency_matrix[vertex][next] = 0;
		dfs(adjacency_matrix, visited, numVertex, next, markers);
	}
	markers[vertex].dfsc = DFSC++;
}

int main() {
	Graph g = {0, 0, NULL};
//	inputGraph(&g);
	readGraph(&g, "graphinput.txt");
	int start;
	printf("Enter starting vertex : ");
	scanf("%d", &start);
	int *visited = (int*)malloc(sizeof(int) * g.numOfVertices);
	Marker *markers = (Marker*)malloc(sizeof(Marker) * g.numOfVertices);
	memset(visited, 0, sizeof(int) * g.numOfVertices);
//	printf("%d ", start);
	printf("Component 1 : ");
	dfs(g.adjacency_matrix, visited, g.numOfVertices, start - 1, markers);
	int i = 0;
	int num = 1;
	for(int i = 0; i < g.numOfVertices; i++) {
		if(visited[i] == 0) {
			printf("\nComponent %d : ", ++num);
			dfs(g.adjacency_matrix, visited, g.numOfVertices, i, markers);
		}
	}
	
	printf("\n");
	for(int i = 0; i < g.numOfVertices; i++) {
		printf("\nVertex %d : %d / %d", i + 1, markers[i].dfs, markers[i].dfsc);
	}
	
	printf("\n");
	for(int i = 0; i < g.numOfVertices; i++) {
		for(int j = 0; j < g.numOfVertices; j++) {
			if(g.adjacency_matrix[i][j] == 1) {
				// forward : dfs1 > dfs2 && dfsc1 > dfsc2
				// backward : dfs1 < dfs2 && dfsc1  > dfsc2
				// cross : dfs1 < dfs2 && dfsc1 < dfsc2
				int dfs1 = markers[i].dfs, dfs2 = markers[j].dfs, 
				dfsc1 = markers[i].dfsc, dfsc2 = markers[j].dfsc;
				printf("\n(%2d, %2d) is a ", i + 1, j + 1);
				if(dfs1 < dfs2 && dfsc1 > dfsc2) {
					printf("forward");
				} else if(dfs1 > dfs2 && dfsc1 < dfsc2) {
					printf("backward");
				} else if(dfs1 > dfs2 && dfsc1 > dfsc2) {
					 printf("cross");
				}
				printf(" edge.");
			}
		}
	}
}
