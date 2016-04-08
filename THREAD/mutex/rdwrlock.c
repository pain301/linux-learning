#include <stdio.h>
#include <pthread.h>

pthread_rwlock_t rwlock;

int counter = 0;

void * thread_write(void *arg){
	int t;
	while(1){
		pthread_rwlock_wrlock(&rwlock);
		t = counter;
		usleep(100);
		printf("write %x counter %d\n",pthread_self(),++t);
		counter = t;
		pthread_rwlock_unlock(&rwlock);
		usleep(1000);
	}
}

void * thread_read(void *arg){
	while(1){
		pthread_rwlock_rdlock(&rwlock);
		printf("read %x counter %d\n",pthread_self(),counter);
		pthread_rwlock_unlock(&rwlock);
		usleep(1);
	}
}

int main(){
	int i;
	pthread_t tid[8];
	pthread_rwlock_init(&rwlock,NULL);
	for(i = 0;i<3;i++){
		pthread_create(&tid[i],NULL,thread_write,NULL);
	}
	for(;i<8;i++){
		pthread_create(&tid[i],NULL,thread_read,NULL);
	}
	for(i=0;i<8;i++){
		pthread_join(tid[i],NULL);
	}
	pthread_rwlock_destroy(&rwlock);
	return 0;
}
