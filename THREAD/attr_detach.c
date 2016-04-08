#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
	

void * thread_fun(void *arg){
	int n = 5;
	while(n--){
		printf("thread id %x times %d\n",(int)pthread_self(),n);
		sleep(1);
	}
	return (void *)1;
}

int main(){
	pthread_t tid;	
	int err;
	void *ret;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	//设置线程属性为分离态
	pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
	pthread_create(&tid,&attr,thread_fun,NULL);
	err = pthread_join(tid,&ret);
	if(err!=0){
		fprintf(stderr,"pthread_join failed %s\n",strerror(err));
		exit(-1);
	}
	printf("%x exit code is %d\n",tid,(int)ret);
	pthread_attr_destroy(&attr);
	return 0;
}
