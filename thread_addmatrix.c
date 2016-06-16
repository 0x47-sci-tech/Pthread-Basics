#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define M 2
#define N 2
#define DEBUG
    
int matrix1[M][N], matrix2[M][N], matrix3[M][N], m, n;

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

void *thread_add_matrix(void *arg) {
    int i, j;
    i = *(int*)arg;
#ifdef DEBUG
    printf("thread for row %lu\t%d\n", pthread_self(), i);
#endif
    for(j = 0; j < N; j++) {
            matrix3[i][j] = matrix1[i][j] + matrix2[i][j];
    }

}
int main() {
    pthread_t thread_id[M];
    int thread[M], i, count = 0;

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
    for (i = 0; i < m; i++) {
        printf("Started %d\n", i);
        thread[i] = pthread_create(&thread_id[i], NULL, thread_add_matrix, (void *)&i);
        usleep(10);
        if (thread[i]) {
            fprintf(stderr, "error %d\n", i);
            getchar();
        }
    }
    for (i = 0; i < m; i++) {
        pthread_join(thread_id[i], NULL);
    }
#ifdef DEBUG
    print_matrix(matrix3, m, n);
#endif

    return 0;
}
