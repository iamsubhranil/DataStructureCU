#include <stdio.h>
#include <stdlib.h>

typedef struct Graph {
	int numOfVertices;
	int numOfEdges;
	int **edges;
} Graph;

void readGraph(Graph *g, const char *file) {
	FILE *f = fopen(file, "r");
	if(f == NULL) {
		printf("Error: Unable to read '%s'!\n", file);
		return;
	}
	fscanf(f, "%d", &g->numOfVertices);
	fscanf(f, "%d", &g->numOfEdges);
	g->edges = (int **)malloc(sizeof(int*) * g->numOfEdges);
	for(int i = 0; i < g->numOfEdges; i++) {
		g->edges[i] = (int*)malloc(sizeof(int) * 2);
		fscanf(f, "%d%d", &(g->edges[i][0]), &(g->edges[i][1]));
	}
	fclose(f);
}

void inputGraph(Graph *g) {
	printf("Enter number of vertices : ");
	scanf("%d", &g->numOfVertices);
	printf("Enter the number of edges : ");
	scanf("%d", &g->numOfEdges);
	g->edges = (int **)malloc(sizeof(int*) * g->numOfEdges);
	for(int i = 0; i < g->numOfEdges; i++) {
		printf("Enter the terminal vertices for edge %d : ", i + 1);
		g->edges[i] = (int*)malloc(sizeof(int) * 2);
		scanf("%d%d", &(g->edges[i][0]), &(g->edges[i][1]));
		if(g->edges[i][0] > v || g->edges[i][1] > v || g->edges[i][0] < 1 || g->edges[i][1] < 1){
			printf("Invalid edge : (%s, %s)\n", a, b);
			i--;
			continue;
		}
	}
}

void printGraph(Graph *g) {
	printf("Number of vertices : %d\n", g->numOfVertices);
	printf("Number of edges : %d\n", g->numOfEdges);
	for(int i = 0; i < g->numOfEdges; i++) {
		printf("Edge %d : (%d, %d)\n", i + 1, g->edges[i][0], g->edges[i][1]);
	}
}

int main() {
	Graph g = {0, 0, NULL};
	readGraph(&g, "graphinput.txt");
	printGraph(&g);
}
