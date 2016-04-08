#include <stdio.h>
#include <pthread.h>

int main(){
	pthread_attr_t attr;
	size_t stacksize;
	void *stackaddr;
	pthread_attr_init(&attr);	
	pthread_attr_getstack(&attr,&stackaddr,&stacksize);
	printf("stacksize = %x\n",stacksize);
	printf("stackaddr = %p\n",stackaddr);
	int detachstate;
	pthread_attr_getdetachstate(&attr,&detachstate);
	if(detachstate==PTHREAD_CREATE_DETACHED){
		printf("pthread created detached\n");
	}else if(detachstate==PTHREAD_CREATE_JOINABLE){
		printf("pthread created joinable\n");
	}else{
		printf("pthread created unknow\n");
	}
	pthread_attr_destroy(&attr);
	return 0;
}
