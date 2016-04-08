#include <stdio.h>
#include <pthread.h>

#define LOOPTIMES 1000 

int counter = 0;
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;

void * thread_fun1(void *arg){
	int i = 0;
	int val;
	while(i++<LOOPTIMES){
		//加锁
		pthread_mutex_lock(&mutex1);
		printf("thread a\n");
		usleep(1);
		pthread_mutex_lock(&mutex2);
		val = counter;
		usleep(100);
		printf("thread id = %x  %d\n",pthread_self(),val+1);
		counter = val+1;
		//解锁
		pthread_mutex_unlock(&mutex1);
		pthread_mutex_unlock(&mutex2);
	}
	return NULL;
}

void * thread_fun2(void *arg){
	int i = 0;
	int val;
	while(i++<LOOPTIMES){
		//加锁
		pthread_mutex_lock(&mutex2);
		printf("thread b\n");
		pthread_mutex_lock(&mutex1);
		val = counter;
		usleep(100);
		printf("thread id = %x  %d\n",pthread_self(),val+1);
		counter = val+1;
		//解锁
		pthread_mutex_unlock(&mutex2);
		pthread_mutex_unlock(&mutex1);
	}
	return NULL;
}

int main(){
	pthread_t tid1;
	pthread_t tid2;
	pthread_create(&tid1,NULL,thread_fun1,NULL);
	pthread_create(&tid2,NULL,thread_fun2,NULL);
	
	pthread_join(tid1,NULL);
	pthread_join(tid2,NULL);
	return 0;
}
