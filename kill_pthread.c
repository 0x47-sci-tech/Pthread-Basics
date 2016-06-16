#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>

#define NUM_THREADS 4
#define N 1000

pthread_mutex_t sum_mutex1;//Mutex for sum variable

int a[N], sum;

void initialize_array(int *a, int n) {
    int i;
    for (i = 1; i <= n; i++) {
        a[i-1] = i;
    }
}
void print_signal(int signum) {
    printf("Signalll.... : signal number : %d\n", signum);
}
void *thread_add(void *argument) {
    int i, thread_sum, arg;
    thread_sum = 0;
    arg = *(int *)argument;
    for (i = ((arg-1)*(N/NUM_THREADS)) ; i < (arg*(N/NUM_THREADS)); i++) {
        thread_sum += a[i];
    }
    printf("Thread : %d : Sum : %d\n", arg, thread_sum);
    pthread_mutex_lock(&sum_mutex1);
    sum += thread_sum;
    pthread_mutex_unlock(&sum_mutex1);
}

void print_array(int *a, int size) {
    int i;
    for(i = 0; i < size; i++) 
        printf(" %d ", a[i]);
    putchar('\n');
}
int main() {
    int i, *range;
    pthread_t thread_id[NUM_THREADS];

    sum = 0;
    initialize_array(a, N);
    printf("Array : \n");
    print_array(a, N);
    signal(SIGALRM, print_signal);

    for (i = 0; i < NUM_THREADS; i++) {
        range = (int *)malloc(sizeof(int));
        *range = (i+1);
        if (pthread_create(&thread_id[i], NULL, thread_add, (void *)range))
            printf("Thread creation failed for thread num : %d\n", *range);
    }

    pthread_kill(thread_id[0], SIGALRM);
    pthread_kill(thread_id[1], SIGALRM);
    for (i = 0; i < NUM_THREADS; i++) {
        pthread_join(thread_id[i], NULL);
    }

    printf("Sum : %d\n", sum);
    return 0;
}
