#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

void * do_thread(void *arg){
	printf("thread id = %x\n",pthread_self());
	printf("process id = %d\n",getpid());
	int *p = (int *)arg;
	printf("arg = %d\n",*p);
}

int main(){
	int err;
	pthread_t tid;
	int arg = 10;
	err = pthread_create(&tid,NULL,do_thread,(void *)&arg);
	if(err!=0){
		fprintf(stderr,"pthread_create failded %s\n",strerror(err));
		exit(-1);
	}
	printf("main thread id = %x\n",pthread_self());
	printf("child thread id = %x\n",tid);
	printf("process id = %d\n",getpid());
	puts("");
	usleep(1);
	//主控线程返回导致进程结束
	return 0;
}
