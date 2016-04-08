#include <stdio.h>
#include <pthread.h>

#define LOOPTIMES 100 

int counter = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void * thread_fun(void *arg){
	int i = 0;
	int val;
	while(i++<LOOPTIMES){
		//加锁
		pthread_mutex_lock(&mutex);
		val = counter;
		usleep(100);
		printf("thread id = %x  %d\n",pthread_self(),val+1);
		counter = val+1;
		//解锁
		pthread_mutex_unlock(&mutex);
	}
	return NULL;
}

int main(){
	pthread_t tid1;
	pthread_t tid2;
	pthread_create(&tid1,NULL,thread_fun,NULL);
	pthread_create(&tid2,NULL,thread_fun,NULL);
	
	pthread_join(tid1,NULL);
	pthread_join(tid2,NULL);
	return 0;
}
