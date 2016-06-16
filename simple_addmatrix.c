#include <stdio.h>
#include <stdlib.h>

#define M 750
#define N 750
void accept_matrix(int matrix[M][N], int m, int n) { 
    int i, j, count;

    count = 0;
#ifndef M
    *matrix = (int **)malloc(sizeof(int *)*m);
    for (i = 0; i < m; i++)
        matrix[i] = (int *)malloc(sizeof(int)*n);
#endif

    for (i = 0; i < m; i++)
        for (j = 0; j < n; j++)
            matrix[i][j] = count++;
}
void  print_matrix(int matrix[M][N], int m, int n) {
    int i, j;

    for (i = 0; i < m; i++) {
        for (j = 0; j < n; j++) 
            printf(" %5d ", matrix[i][j]);
        putchar('\n');
    }
}

void add_matrix(int matrix1[M][N], int matrix2[M][N], int matrix3[M][N]) {
    int i, j;
    for(i = 0; i < M; i++) {
        for(j = 0; j < N; j++) {
            matrix3[i][j] = matrix1[i][j] + matrix2[i][j];
        }
    }

}
int main() {
    int matrix1[M][N], matrix2[M][N], matrix3[M][N], m, n;

#ifndef M
    printf("Enter the size of matrix (mxn): ");
    scanf("%d", &m);
    scanf("%d", &n);
#else
    m = M;
    n = N;
#endif

    printf("\nMatrix 1:\n");
    accept_matrix(matrix1, m, n);
#ifdef DEBUG
    print_matrix(matrix1, m, n);
#endif

    printf("\nMatrix 2:\n");
    accept_matrix(matrix2, m, n);
#ifdef DEBUG
    print_matrix(matrix2, m, n);
#endif

    printf("\nAdded Matrix 3:\n");
    add_matrix(matrix1, matrix2, matrix3);
#ifdef DEBUG
    print_matrix(matrix3, m, n);
#endif

    return 0;
}
