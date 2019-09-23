#include <stdio.h>
#include <string.h>

int findVertexNum(char vertexMap[][100], char *toFind) {
	for(int i = 0; i < 100; i++) {
		if(strcmp(vertexMap[i], toFind) == 0)
			return i + 1;
	}
	return -1;
}

int findMaxLength(char vertexMap[][100], int vertices) {
	int len = 0, temp = 0;
	for(int i = 0; i < vertices; i++) {
		if((temp = strlen(vertexMap[i]) > len))
			len = temp;
	}
	return len;
}

int numberOfDigits(int n) {
	int d = 1;
	while(n > 10) {
		n /= 10;
		d++;
	}
	return d;
}

int main() {
	int  mat[100][100];
	char vertexMap[100][100];
	int  v, e;
	char a[100], b[100];
	printf("Enter number of vertices : ");
	scanf("%d", &v);
	if(v >= 100) {
		printf("Number of vertices is out of range!");
		return 1;
	}
	printf("Enter the name of the vertices : ");
	for(int i = 0; i < v; i++) {
		printf("\nV%d : ", i + 1);
		scanf("%s", vertexMap[i]);
	}
	printf("Enter the number of edges : ");
	scanf("%d", &e);

	for(int i = 0; i < e; i++) {
		printf("Enter the terminal vertices for edge %d : ", i + 1);
		scanf("%s%s", a, b);
		int aNum = findVertexNum(vertexMap, a);
		int bNum = findVertexNum(vertexMap, b);
		if(aNum > v || bNum > v || aNum < 1 || bNum < 1) {
			printf("Invalid edge : (%s, %s)\n", a, b);
			i--;
			continue;
		}
		aNum--;
		bNum--;
		mat[aNum][bNum] = mat[bNum][aNum] = 1;
	}
	int width = findMaxLength(vertexMap, v);
	printf("The adjacency matrix : \n\t");
	for(int i = 0; i < v; i++) {
		printf("%*s\t", width, vertexMap[i]);
	}
	printf("\n");
	for(int i = 0; i < v; i++) {
		printf("%*s\t", width, vertexMap[i]);
		for(int j = 0; j < v; j++) {
			printf("%*d\t", width, mat[i][j]);
		}
		printf("\n\n");
	}
	printf("\n");
	return 0;
}
