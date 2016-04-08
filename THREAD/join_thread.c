#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *thread1(void *arg){
	printf("thread1 running\n");
	return (void *)1;
}

void *thread2(void *arg){
	printf("thread2 running\n");
	pthread_exit((void *)2);
}

void *thread3(void *arg){
	while(1){
		printf("thread3 running\n");
		sleep(1);
	}
}

int main(){
	pthread_t tid;
	
	void *ret;

	pthread_create(&tid,NULL,thread1,NULL);
	//阻塞回收线程，退出值保存在ret中
	pthread_join(tid,&ret);
	printf("thread1 exit code = %d\n",(int)ret);

	pthread_create(&tid,NULL,thread2,NULL);
	pthread_join(tid,&ret);
	printf("thread2 exit code = %d\n",(int)ret);

	pthread_create(&tid,NULL,thread3,NULL);
	sleep(3);
	//主控线程取消一个线程
	pthread_cancel(tid);
	pthread_join(tid,&ret);
	printf("thread3 exit code = %d\n",(int)ret);

	return 0;
}
