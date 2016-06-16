#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define M  3
#define N  2
#define P  4
    
int matrix1[M][N], matrix2[N][P], result[M][P];

void accept_matrix1(int matrix[M][N]) { 
    int i, j, count;

    count = 0;
    for (i = 0; i < M; i++)
        for (j = 0; j < N; j++)
            matrix[i][j] = count++;
}

void accept_matrix2(int matrix[N][P]) { 
    int i, j, count;

    count = 0;
    for (i = 0; i < N; i++)
        for (j = 0; j < P; j++)
            matrix[i][j] = count++;
}
void  print_matrix1(int matrix[M][N]) {
    int i, j;

    for (i = 0; i < M; i++) {
        for (j = 0; j < N; j++) 
            printf(" %5d ", matrix[i][j]);
        putchar('\n');
    }
    putchar('\n');
}
void  print_matrix2(int matrix[N][P]) {
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < P; j++) 
            printf(" %5d ", matrix[i][j]);
        putchar('\n');
    }
    putchar('\n');
}
void  print_matrix3(int matrix[M][P]) {
    int i, j;

    for (i = 0; i < M; i++) {
        for (j = 0; j < P; j++) 
            printf(" %5d ", matrix[i][j]);
        putchar('\n');
    }
    putchar('\n');
}
void *thread_mult_matrix(void *arg) {
    int i, j, k;
    i = *(int *)arg;
    for (j = 0; j < N; j++) 
        for (k = 0; k < P; k++)
            result[i][k] += matrix1[i][j]*matrix2[j][k];
}

int main() {
    int i;
    int *a;
    pthread_t thread_id[M];

    accept_matrix1(matrix1);
    accept_matrix2(matrix2);
    for (i = 0; i < M; i++) {
        a = (int *)malloc(sizeof(int));
        *a = i;
        pthread_create(&thread_id[i], NULL, thread_mult_matrix, (void *)&a);
    }
    
    for (i = 0; i < M; i++) {
        pthread_join(thread_id[i], NULL);
        free(a);
    }
    
    printf("Matrix 1\n");
    print_matrix1(matrix1);
    printf("Matrix 2\n");
    print_matrix2(matrix2);
    printf("Multiplication\n");
    print_matrix3(result);

    return 0;
}
