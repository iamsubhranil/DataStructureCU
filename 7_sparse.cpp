#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Matrix {
	int **mat, r, c;
} Matrix;

Matrix create_matrix(int r, int c) {
	int **ret = (int **)malloc(sizeof(int *) * r);
	for(int i = 0; i < r; i++) {
		ret[i] = (int *)malloc(sizeof(int) * c);
		memset(ret[i], 0, sizeof(int) * c);
	}
	Matrix m = {ret, r, c};
	return m;
}

void print_mat(Matrix m) {
	printf("\n__%*s__", m.c * 3 - 1, " ");
	for(int i = 0; i < m.r; i++) {
		printf("\n| %2d", m.mat[i][0]);
		for(int j = 1; j < m.c; j++) {
			printf(" %2d", m.mat[i][j]);
		}
		printf(" |");
	}
	printf("\n--%*s--\n", m.c * 3 - 1, " ");
}

Matrix convert_to_sparse(Matrix input) {
	// count the number of non zero elements
	int count = 0;
	for(int i = 0; i < input.r; i++) {
		for(int j = 0; j < input.c; j++) {
			if(input.mat[i][j])
				count++;
		}
	}
	Matrix sparse    = create_matrix(count + 1, 3);
	sparse.mat[0][0] = input.r;
	sparse.mat[0][1] = input.c;
	sparse.mat[0][2] = count;
	int k            = 1;
	for(int i = 0; i < input.r; i++) {
		for(int j = 0; j < input.c; j++) {
			if(input.mat[i][j]) {
				sparse.mat[k][0] = i;
				sparse.mat[k][1] = j;
				sparse.mat[k][2] = input.mat[i][j];
				k++;
			}
		}
	}
	return sparse;
}

Matrix ordered_transpose(Matrix sparse) {
	Matrix transpose    = create_matrix(sparse.r, sparse.c);
	transpose.mat[0][0] = sparse.mat[0][1];
	transpose.mat[0][1] = sparse.mat[0][0];
	transpose.mat[0][2] = sparse.mat[0][2];
	int ptr             = 1;
	for(int j = 0; j < sparse.mat[0][1]; j++) {
		for(int i = 1; i < sparse.r; i++) {
			// Check if (i, j) exists in the sparse
			if(sparse.mat[i][1] == j) {
				// it does. so put (j, i) in transpose
				transpose.mat[ptr][0] = j;
				transpose.mat[ptr][1] = sparse.mat[i][0];
				transpose.mat[ptr][2] = sparse.mat[i][2];
				ptr++;
			}
		}
	}
	return transpose;
}

Matrix input_sparse_matrix(bool fromFile, const char *fileName) {
	FILE *f;
	if(fromFile) {
		f = fopen(fileName, "r");
		if(f == NULL) {
			printf("\n[Error] Unable to open '%s'!", fileName);
			exit(1);
		}
	} else {
		f = stdin;
	}
	int r, c, count = 0;
	if(!fromFile)
		printf("Enter number of rows : ");
	fscanf(f, "%d", &r);
	if(!fromFile)
		printf("Enter number of columns : ");
	fscanf(f, "%d", &c);
	Matrix m = create_matrix(r, c);

	for(int i = 0; i < m.r; i++) {
		if(!fromFile)
			printf("\nEnter row %d : ", i + 1);
		for(int j = 0; j < m.c; j++) {
			fscanf(f, "%d", &m.mat[i][j]);
			if(m.mat[i][j]) {
				count++;
			}
		}
	}
	if(fromFile) {
		fclose(f);
	}

	if((double)(m.r * m.c * 0.34) <= count) {
		printf("\n[Error] More than one third non-zero elements present in the "
		       "matrix (%g%%)!",
		       (double)(count * 100) / (m.r * m.c));
		printf("\n[Error] Unable to represent the matrix in sparse format!");
		exit(2);
	}
	return m;
}

int main() {
	Matrix mat = input_sparse_matrix(true, "sparseinput.txt");
	printf("Given matrix : ");
	print_mat(mat);
	Matrix sparse = convert_to_sparse(mat);
	printf("Corresponding sparse matrix : ");
	print_mat(sparse);
	Matrix transpose = ordered_transpose(sparse);
	printf("Transpose sparse matrix : ");
	print_mat(transpose);
}
