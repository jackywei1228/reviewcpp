#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/syscall.h>
#define gettid() syscall(SYS_gettid)
 
pthread_mutex_t mutex;

static int gI = 0;

__thread int gVar = 0;

void *print_msg(void *arg){
        int i=0;
        pthread_mutex_lock(&mutex);
        for(i=0;i<15;i++){
                printf("%d : output : %d\n",gettid(),i);
                usleep(100);
        }
        pthread_mutex_unlock(&mutex);
}

void *print_msg_extern(void *arg){
        int i=0;
        // pthread_mutex_lock(&mutex);
        for(i=0;i<15;i++){
            printf("%d : ------------- gI output ------------\n",gettid());
            printf("%d :gI output : %d\n",gettid(),gI++);
            printf("%d :gVar output : %d\n",gettid(),gVar++);
            printf("%d : ************* gI output *************\n",gettid());
        }
        // pthread_mutex_unlock(&mutex);
}

int main(int argc,char** argv){
        pthread_t id1;
        pthread_t id2;
        pthread_mutex_init(&mutex,NULL);
        pthread_create(&id1,NULL,print_msg_extern,NULL);
        pthread_create(&id2,NULL,print_msg_extern,NULL);
        pthread_join(id1,NULL);
        pthread_join(id2,NULL);
        pthread_mutex_destroy(&mutex);
        return 1;
}

