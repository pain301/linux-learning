#include <stdio.h>
#include <signal.h>

void * thread_fun(void *arg){

}

int main(){
	pthread_t tid1;
	pthread_t tid2;

	pthread_create(&tid1,NULL,thread_fun,NULL);
	pthread_create(&tid2,NULL,thread_fun,NULL);
	if(pthread_equal(tid1,tid2)){
		printf("thread equal\n");
	}else{
		printf("thread unequal\n");
	}	
	return 0;
}
