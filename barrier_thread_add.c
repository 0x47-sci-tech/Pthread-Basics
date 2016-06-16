#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 4
#define N 1000

pthread_barrier_t sum_barrier;//barrier
pthread_mutex_t sum_mutex1;//Mutex for sum variable

int a[N], sum;

void initialize_array(int *a, int n) {
    int i;
    for (i = 1; i <= n; i++) {
        a[i-1] = i;
    }
}
void i_got_serial(int arg) {
    printf("I'm such lucky thread, I got serial : %d\n", arg);
}

void *thread_add(void *argument) {
    int i, thread_sum, arg, ret;
    thread_sum = 0;
    arg = *(int *)argument;

    for (i = ((arg-1)*(N/NUM_THREADS)) ; i < (arg*(N/NUM_THREADS)); i++) {
        thread_sum += a[i];
    }
    ret = pthread_barrier_wait(&sum_barrier);

    printf("----------->barrier crossed : thread : %d<---------------\n", arg);
    sleep(1);
    if (ret == PTHREAD_BARRIER_SERIAL_THREAD) {
        printf("Thread : %d : Sum : %d\n", arg, thread_sum);
        i_got_serial(arg);
    }
    else if (ret == 0)
        printf("Thread : %d : Sum : %d\n", arg, thread_sum);
    else
        printf("Barrier failed in thread: %d\n", arg);

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

void new_parent() {
    int i, *range;
    pthread_t thread_id[NUM_THREADS];

    sum = 0;
    initialize_array(a, N);


    printf("New Parent : Array : \n");
    print_array(a, N);

    for (i = 0; i < NUM_THREADS; i++) {
        range = (int *)malloc(sizeof(int));
        *range = (i+1);
        if (pthread_create(&thread_id[i], NULL, thread_add, (void *)range))
            printf("New Parent : Thread creation failed for thread num : %d\n", *range);
    }
    
    for (i = 0; i < NUM_THREADS; i++) {
        pthread_join(thread_id[i], NULL);
    }

    printf("New Parent : Sum : %d\n", sum);
}
int main() {
    int i, *range;
    pthread_t thread_id[NUM_THREADS];

    sum = 0;
    initialize_array(a, N);

    pthread_barrier_init(&sum_barrier, NULL, NUM_THREADS);

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

    printf("Main : Sum : %d\n", sum);
    return 0;
}
