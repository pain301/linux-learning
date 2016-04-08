#include <signal.h>
#include <stdio.h>

void do_sig(int num,siginfo_t *siginfo,void *reserve){
	printf("sig num is %d\n",num);
	int i = 0;
	while(i<6){
		i++;
		printf("do_sig %d times\n",i);
		sleep(1);
	}
}

int main(){
	struct sigaction act;
	act.sa_sigaction = do_sig;
	//使用sa_sigaction的函数指针
	act.sa_flags = SA_SIGINFO;
	sigemptyset(&act.sa_mask);
	sigaddset(&act.sa_mask,SIGQUIT);
	sigaction(SIGINT,&act,NULL);
	while(1){
		printf("******************\n");
		sleep(1);
	}
	return 0;
}
