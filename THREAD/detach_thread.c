#include <stdio.h>
#include <signal.h>

void * thread_fun(void *arg){
	int i = 0;
	while(i++<3){
		printf("thread loop times %d\n",i);
		sleep(1);
	}
	return (void *)100;
}

int main(){
	pthread_t tid;
	pthread_create(&tid,NULL,thread_fun,NULL);
	//pthread_detach(tid);
	void *ret;
	int err;
	while(1){
		err = pthread_join(tid,&ret);
		if(err!=0){
			fprintf(stderr,"thread %s\n",strerror(err));
		}else{
			fprintf(stderr,"thread exit code %d\n",(int)ret);
		}
		sleep(1);
	}
	return 0;
}
