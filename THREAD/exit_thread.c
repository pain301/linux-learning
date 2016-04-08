#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void * do_thread1(void *arg){
	char *p = (char *)arg;
	int i = 0;
	while(i<6){
		printf("%s thread1\n",p);
		if(i++==3){
			pthread_exit(1);
		}
	}
}

void * do_thread2(void *arg){
	char *p = (char *)arg;
	int i = 0;
	for(;i<3;i++){
		printf("%s thread2\n",p);
		sleep(1);
	}
	pthread_exit(2);
}

int main(){
	pthread_t tid1;
	pthread_t tid2;
	int err;
	err = pthread_create(&tid1,NULL,do_thread1,"thread1");
	if(err!=0){
		fprintf(stderr,"pthread_create failed %s\n",strerror(err));
		exit(-1);
	}
	err = pthread_create(&tid1,NULL,do_thread2,"thread2");
	if(err!=0){
		fprintf(stderr,"pthread_create failed %s\n",strerror(err));
		exit(-1);
	}
	printf("main thread\n");
	//只是导致主线程退出，进程并不会终止
	pthread_exit(3);
	return 0;
}
