#include <stdio.h> 
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>

void  *print_coep(void *arg) {
    printf("%s COEP\n", (char *)arg);
    sleep(5);
    printf("I'm in %lu\n", pthread_self());
}
int main() {
    int iret;
    pthread_t t1, t2;
    char *arg1 = "Hello", *arg2= "happy";

    
    printf("Both thread stared\n");
    if(pthread_create(&t1, NULL, print_coep, arg1)){
        fprintf(stderr, "create failed\n");
        exit(-1);
    }
    if(pthread_create(&t2, NULL, print_coep, arg2)){
        fprintf(stderr, "create failed\n");
        exit(-1);
    }
    printf("I'm here before pthread_exit\n");
    exit(-1);
    printf("I'm here after pthread_exit\n");
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    printf("Both thread overs\n");
    printf("I'm main %lld\n", getpid());

    return 0;
}
 
