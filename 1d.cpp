#include <stdio.h>

int main() {
	int mat[100][100];
	int v, e,a , b, w;
	printf("Enter number of vertices : ");
	scanf("%d", &v);
	printf("enter the number of edges : ");
	scanf("%d", &e);
	if(v >= 100) {
		printf("Number of vertices is out of range!");
		return 1;
	}
	
	for(int i = 0; i < e; i++) {
		printf("Enter the terminal vertices for edge %d : ", i + 1);
		scanf("%d%d", &a, &b);
		if(a > v || b > v || a < 1 || b < 1){
			printf("Invalid edge : (%d, %d)\n", a, b);
			i--;
			continue;
		}
		printf("Enter the weight for edge %d : ", i + 1);
		scanf("%d", &w);
		a--;
		b--;
		mat[a][b] = w;
	}
	
	printf("The adjacency matrix : \n\t");
	for(int i = 0; i < v; i++) {
		printf("v%2d\t", i + 1);
	}
	printf("\n");
	for(int i = 0;i < v;i++){
		printf("v%2d\t", i + 1);
		for(int j = 0; j < v;j++) {
			printf("%2d\t", mat[i][j]);
		}
		printf("\n\n");
	}
	printf("\n");
	return 0;
}
