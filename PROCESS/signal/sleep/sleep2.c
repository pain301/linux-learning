#include <signal.h>
#include <stdio.h>

void handle_sig(int signo){

}

unsigned int mysleep(unsigned int secs){
	struct sigaction newact;
	struct sigaction oldact;
	newact.sa_handler = handle_sig;
	sigemptyset(&newact.sa_mask);
	sigaddset(&newact.sa_mask,SIGQUIT);
	newact.sa_flags = 0;
	sigaction(SIGALRM,&newact,&oldact);
	alarm(secs);
	//可能在pause之前发生调度，造成pause语句再也执行不了
	pause();
	//恢复SIGALRM的原有的处理动作
	sigaction(SIGALRM,&oldact,NULL);
	int unsleep = alarm(0);
	return unsleep;
}

int main(){
	int i = 0;
	while(1){
		printf("sleep time %d\n",i++);
		mysleep(1);
	}
	return 0;
}
