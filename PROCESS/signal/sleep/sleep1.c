#include <signal.h>
#include <stdio.h>

void handle_sig(int signo){
}

void mysleep(unsigned int secs){
	signal(SIGALRM,handle_sig);
	//若程序在执行此句之后，CPU调度，则可能到时pause无法收到SIGALRM信号
	//造成永久等待
	alarm(secs);
	pause();
}

int main(){
	int i = 0;
	while(1){
		mysleep(2);
		printf("sleep time = %d\n",i++);
	}
	return 0;
}
