#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

void clean(void *arg){
	free(arg);
	printf("clean heap memory\n");
}

void * thread_fun(void *arg){
	char *p = malloc(10);
	pthread_cleanup_push(clean,p);
	int i = 5;
	while(i--){
		printf("thread_fun\n");	
		sleep(1);
	}
	pthread_exit((void *)3);
	pthread_cleanup_pop(0);
}

int main(){
	pthread_t tid;
	pthread_create(&tid,NULL,thread_fun,NULL);
	printf("main thread\n");
	sleep(1);
	//pthread_cancel(tid);
	void *ret;
	pthread_join(tid,&ret);
	printf("thread return val %d\n",(int)ret);
	return 0;
}
