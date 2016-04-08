#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define NUM 5

sem_t empty;
sem_t full;

int queue[NUM];

void * consume(void *arg){
	int cur = 0;
	while(1){
		sem_wait(&full);
		printf("consume product %d\n",queue[cur]);
		cur = (cur+1)%NUM;
		sem_post(&empty);
		sleep(rand()%5);
	}
}

void * produce(void *arg){
	int cur = 0;
	while(1){
		sem_wait(&empty);
		queue[cur] =rand()%100+1; 
		printf("produce product %d\n",queue[cur]);
		cur = (cur+1)%NUM;
		sem_post(&full);
		sleep(rand()%5);
	}
}

int main(){
	pthread_t tid1;
	pthread_t tid2;
	
	sem_init(&empty,0,5);
	sem_init(&full,0,0);
	srand(time(NULL));
	pthread_create(&tid1,NULL,consume,NULL);
	pthread_create(&tid2,NULL,produce,NULL);
		
	pthread_join(tid1,NULL);
	pthread_join(tid2,NULL);
	
	sem_destroy(&empty);
	sem_destroy(&full);
	return 0;
}
