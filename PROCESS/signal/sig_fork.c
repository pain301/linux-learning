#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

void handle_sig(int signo){
	printf("pid = %d get signal %d\n",getpid(),signo);
}

int main(){
	struct sigaction act;
	act.sa_handler = handle_sig;
	act.sa_flags = 0;
	sigemptyset(&act.sa_mask);
	sigaddset(&act.sa_mask,SIGQUIT);
	sigaction(SIGUSR1,&act,NULL);
	pid_t pid = fork();
	if(pid<0){
		perror("fork error");
		exit(-1);
	}
	if(pid==0){
		while(1){
			kill(getppid(),SIGUSR1);
			sleep(10);
		}
	}else if(pid>0){
		while(1){
			kill(pid,SIGUSR1);
			sleep(10);
		}
	}
	return 0;
}
