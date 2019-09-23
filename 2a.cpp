#include <stdio.h>
#include <stdlib.h>

typedef struct Graph {
	int   numOfVertices;
	int   numOfEdges;
	int **edges;
} Graph;

void generateIncidenceMatrix(Graph *g) {
	int mat[100][100];

	for(int i = 0; i < g->numOfEdges; i++) {
		int a = g->edges[i][0], b = g->edges[i][1];
		a--;
		b--;
		mat[a][i] = 1;
		mat[b][i] = 1;
	}

	printf("The incidence matrix : \n\t");
	for(int i = 0; i < g->numOfEdges; i++) {
		printf("e%2d\t", i + 1);
	}
	printf("\n");
	for(int i = 0; i < g->numOfVertices; i++) {
		printf("v%2d\t", i + 1);
		for(int j = 0; j < g->numOfEdges; j++) {
			printf("%2d\t", mat[i][j]);
		}
		printf("\n\n");
	}
	printf("\n");
}

void inputGraph(Graph *g) {
	printf("Enter number of vertices : ");
	scanf("%d", &g->numOfVertices);
	printf("Enter the number of edges : ");
	scanf("%d", &g->numOfEdges);
	g->edges = (int **)malloc(sizeof(int *) * g->numOfEdges);
	for(int i = 0; i < g->numOfEdges; i++) g->edges[i] = NULL;
	for(int i = 0; i < g->numOfEdges; i++) {
		printf("Enter the terminal vertices for edge %d : ", i + 1);
		if(!g->edges[i])
			g->edges[i] = (int *)malloc(sizeof(int) * 2);
		scanf("%d%d", &(g->edges[i][0]), &(g->edges[i][1]));
		if(g->edges[i][0] > g->numOfVertices ||
		   g->edges[i][1] > g->numOfVertices || g->edges[i][0] < 1 ||
		   g->edges[i][1] < 1) {
			printf("Invalid edge : (%d, %d)\n", g->edges[i][0], g->edges[i][1]);
			i--;
			continue;
		}
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
	g->edges = (int **)malloc(sizeof(int *) * g->numOfEdges);
	for(int i = 0; i < g->numOfEdges; i++) {
		g->edges[i] = (int *)malloc(sizeof(int) * 2);
		fscanf(f, "%d%d", &(g->edges[i][0]), &(g->edges[i][1]));
	}
	fclose(f);
}

void printGraph(Graph *g) {
	printf("Number of vertices : %d\n", g->numOfVertices);
	printf("Number of edges : %d\n", g->numOfEdges);
	for(int i = 0; i < g->numOfEdges; i++) {
		printf("Edge %d : (%d, %d)\n", i + 1, g->edges[i][0], g->edges[i][1]);
	}
}

int main() {
	Graph g;
	inputGraph(&g);
	// readGraph(&g, "graphinput.txt");
	// printGraph(&g);
	generateIncidenceMatrix(&g);
	return 0;
}
