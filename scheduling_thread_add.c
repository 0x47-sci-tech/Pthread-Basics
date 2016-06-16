#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

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
//Scheduling attributes
    pthread_attr_t custom_sched_attr;   
    int fifo_max_prio, fifo_min_prio, fifo_mid_prio;   
    struct sched_param fifo_param;

//System initialization
    sum = 0;
    initialize_array(a, N);

//Attribute initialization
    pthread_attr_init(&custom_sched_attr);
    pthread_attr_setscope(&custom_sched_attr, PTHREAD_SCOPE_SYSTEM);
    pthread_attr_init(&custom_sched_attr);  
    pthread_attr_setinheritsched(&custom_sched_attr, PTHREAD_EXPLICIT_SCHED);     
    pthread_attr_setschedpolicy(&custom_sched_attr, SCHED_FIFO);  

    fifo_max_prio = sched_get_priority_max(SCHED_FIFO);   
    fifo_min_prio = sched_get_priority_min(SCHED_FIFO);   
    fifo_mid_prio = (fifo_min_prio + fifo_max_prio)/2;    
//  fifo_param.sched_priority = fifo_mid_prio;    
    fifo_param.sched_priority = fifo_max_prio;    

    pthread_attr_setschedparam(&custom_sched_attr, &fifo_param);

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

    printf("Sum : %d\n", sum);
    pthread_attr_destroy(&custom_sched_attr);
    return 0;
}
