#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

struct msg{
	int num;
	struct msg* next;
};

//全局资源
struct msg *head;

//互斥锁
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
//条件变量
pthread_cond_t has_product = PTHREAD_COND_INITIALIZER;

void *product(void *arg){
	struct msg *tmp;
	while(1){
		tmp = malloc(sizeof(struct msg));
		tmp->num = rand()%1000+1;
		printf("produce %d\n",tmp->num);
		//加锁
		pthread_mutex_lock(&lock);
		tmp->next = head;
		head = tmp;
		//解锁
		pthread_mutex_unlock(&lock);
		//唤醒等待队列的队首进程
		pthread_cond_signal(&has_product);
		sleep(rand()%5);
	}
}

void *consume(void *arg){
	struct msg *tmp;
	while(1){
		//加锁
		pthread_mutex_lock(&lock);
		if(head==NULL)
			//1.阻塞，等待被唤醒
			//2.释放互斥锁
			//3.当被唤醒，解除阻塞，重新申请获取互斥锁
			pthread_cond_wait(&has_product,&lock);
		tmp = head;
		head = head->next;
		//解锁
		pthread_mutex_unlock(&lock);
		printf("consume %d\n",tmp->num);
		free(tmp);
		sleep(rand()%5);
	}
}

int main(){
	pthread_t product_id;
	pthread_t consume_id;
	srand(time(NULL));
	pthread_create(&product_id,NULL,product,NULL);
	pthread_create(&consume_id,NULL,consume,NULL);
	pthread_join(product_id,NULL);
	pthread_join(consume_id,NULL);
	return 0;
}
