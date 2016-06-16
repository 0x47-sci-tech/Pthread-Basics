#include <stdio.h> 
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>

void  *print_coep(void *arg) {
    printf("%s COEP\n", (char *)arg);
    printf("I'm in %lu\n", pthread_self());
}
int main() {
    int iret, i;
    pthread_t t[2];
    char *arg[2] = { "Hello", "happy"};

    
    printf("Both thread stared\n");
    for  (i = 0; i < 2; i++) {
    if(pthread_create(&t[i], NULL, print_coep, arg[i])){
        fprintf(stderr, "create failed\n");
        exit(-1);
    }
    }
    for(i = 0; i<2;i++)
        pthread_join(t[i], NULL);
    printf("Both thread overs\n");
    printf("I'm main %lld\n", getpid());

    return 0;
}
 
