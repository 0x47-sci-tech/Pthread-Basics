#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 4
#define N 1000

pthread_rwlock_t sum_rwlock=PTHREAD_RWLOCK_INITIALIZER;

int a[N], sum;

void initialize_array(int *a, int n) {
    int i;
    for (i = 1; i <= n; i++) {
        a[i-1] = i;
    }
}
void *thread_add(void *argument) {
    int i, thread_sum, arg;
    thread_sum = 0;
    arg = *(int *)argument;
    for (i = ((arg-1)*(N/NUM_THREADS)) ; i < (arg*(N/NUM_THREADS)); i++) {
        thread_sum += a[i];
    }
    pthread_rwlock_wrlock(&sum_rwlock);
    printf("Writer : Thread : %d : Sum : %d\n", arg, thread_sum);
    sum += thread_sum;
    pthread_rwlock_unlock(&sum_rwlock);
    sleep(2);
}

void *read_periodic_sum(void *argument){
    int i=0;
    while(i < 2) {
        pthread_rwlock_rdlock(&sum_rwlock);
        printf("Reader : Periodic sum : thread : %ld : %d\n", (long)argument, sum);
        pthread_rwlock_unlock(&sum_rwlock);
        i++;
        sleep(1);
    }
}
void print_array(int *a, int size) {
    int i;
    for(i = 0; i < size; i++) 
        printf(" %d ", a[i]);
    putchar('\n');
}
int main() {
    int i, *range;
    pthread_t thread_id[NUM_THREADS * 2];

    sum = 0;
    initialize_array(a, N);
    printf("Array : \n");
    print_array(a, N);

    for (i = 0; i < NUM_THREADS; i++) {
        range = (int *)malloc(sizeof(int));
        *range = (i+1);
        if (pthread_create(&thread_id[i], NULL, thread_add, (void *)range))
            printf("Thread creation failed for thread num : Writer : %d\n", *range);
    }
    for (; i < NUM_THREADS + NUM_THREADS; i++)
        if (pthread_create(&thread_id[i], NULL, read_periodic_sum, (void *)i))
            printf("Thread creation failed for thread num : Reader : %d\n", *range+1);

    for (i = 0; i < NUM_THREADS; i++) {
        pthread_join(thread_id[i], NULL);
    }
    pthread_join(thread_id[i], NULL);

    printf("Main : Sum : %d\n", sum);
    return 0;
}
