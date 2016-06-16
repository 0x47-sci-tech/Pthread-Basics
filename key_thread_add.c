#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 4
#define N 1000

pthread_mutex_t sum_mutex1;//Mutex for sum variable
pthread_key_t psum_key;//Key 
int a[N], sum;

void initialize_array(int *a, int n) {
    int i;
    for (i = 1; i <= n; i++) {
        a[i-1] = i;
    }
}

void print_partial_sum (int thread_no) {
    int *psum;

    psum = pthread_getspecific(psum_key);
    printf("Partial sum of thread  %d is equal to %d\n", thread_no, *psum);
}

void *thread_add(void *argument) {
    int i, thread_sum, arg;
    int *thread_sum_heap;
    thread_sum = 0;
    arg = *(int *)argument;
    for (i = ((arg-1)*(N/NUM_THREADS)) ; i < (arg*(N/NUM_THREADS)); i++) {
        thread_sum += a[i];
    }
    thread_sum_heap = (int *)malloc(sizeof(int));
    *thread_sum_heap = thread_sum;

    pthread_setspecific(psum_key, (void *)thread_sum_heap);
    print_partial_sum(arg); // it is going to use the psum_key to print the partial sum

    pthread_mutex_lock(&sum_mutex1);
    sum += thread_sum;
    pthread_mutex_unlock(&sum_mutex1);

}
void free_all(int *p) {
    free(p);
}
void initialize_keys () {
    pthread_key_create(&psum_key, (void *)free_all);
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
    initialize_keys();

    printf("Array : \n");
    print_array(a, N);

    for (i = 0; i < NUM_THREADS; i++) {
        range = (int *)malloc(sizeof(int));
        *range = (i+1);
        if (pthread_create(&thread_id[i], NULL, thread_add, (void *)range))
            printf("Thread creation failed for thread num : %d\n", *range);
    }

    for (i = 0; i < NUM_THREADS; i++) {
        pthread_join(thread_id[i], NULL);
    }
    pthread_key_delete(psum_key);

    printf("Sum : %d\n", sum);
    return 0;
}
