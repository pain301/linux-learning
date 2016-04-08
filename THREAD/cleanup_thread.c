#include <stdio.h>
#include <pthread.h>

void clean_fun(void *arg){
	printf("clean fun %d\n",(int)arg);
}

void * thread_fun(void *arg){
	pthread_cleanup_push(clean_fun,(void *)100);
	pthread_cleanup_push(clean_fun,(void *)200);
	printf("thread_fun begining\n");
	sleep(2);
	//使用pthread_exit退出时，无论参数是否为0，都会调用clean_fun函数
	pthread_exit(NULL);
	pthread_cleanup_pop(0);
	pthread_cleanup_pop(0);
	printf("thread_fun ending\n");
}

int main(){
	pthread_t tid;
	pthread_create(&tid,NULL,thread_fun,NULL);
	pthread_join(tid,NULL);
	return 0;
}
