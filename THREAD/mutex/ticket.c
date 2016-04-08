#include <stdio.h>
#include <pthread.h>

pthread_mutex_t lock;
pthread_cond_t tick;

int tickets = 10;

void *window1(void *arg){
	while(1){
		pthread_mutex_lock(&lock);
		if(tickets>0){
			printf("window1 sale a ticket, %d remains\n",--tickets);
		}else{
			pthread_cond_signal(&tick);
		}
		pthread_mutex_unlock(&lock);
		sleep(1);
	}
}

void *window2(void *arg){
	while(1){
		pthread_mutex_lock(&lock);
		if(tickets>0){
			printf("window2 sale a ticket, %d remains\n",--tickets);
		}else{
			pthread_cond_signal(&tick);
		}
		pthread_mutex_unlock(&lock);
		sleep(1);
	}
}

void *window3(void *arg){
	pthread_mutex_lock(&lock);
	if(tickets>0){
		pthread_cond_wait(&tick,&lock);
	}
	tickets = 10;
	pthread_mutex_unlock(&lock);
	pthread_exit(NULL);
}

int main(){
	pthread_t pid1;
	pthread_t pid2;
	pthread_t pid3;
	pthread_mutex_init(&lock,NULL);
	pthread_cond_init(&tick,NULL);
	pthread_create(&pid1,NULL,window1,NULL);
	pthread_create(&pid2,NULL,window2,NULL);
	pthread_create(&pid3,NULL,window3,NULL);

	pthread_join(pid1,NULL);
	pthread_join(pid2,NULL);
	pthread_join(pid3,NULL);

	pthread_mutex_destroy(&lock);
	pthread_cond_destroy(&tick);
	return 0;
}
