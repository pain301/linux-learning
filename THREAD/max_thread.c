#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void * do_thread(void *arg){
	while(1){
		sleep(1);
	}
}

int main(){
	pthread_t tid;
	int err;
	int arg = 10;
	int i = 1;
	while(1){
		err = pthread_create(&tid,NULL,do_thread,(void *)(&arg));
		if(err!=0){
			fprintf(stderr,"pthread_create failed %s\n",
					strerror(err));
			exit(-1);
		}
		printf("already created %d threads\n",i++);
	}
	return 0;
}
