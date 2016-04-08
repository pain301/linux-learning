#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void print_err(const char * ,int);

void do_sig_child(int signo){
	pid_t pid;
	int status;
	while((pid = waitpid(0,&status,WNOHANG))>0){
		//正常退出
		if(WIFEXITED(status)){
			printf("child %d exit %d\n",pid,WEXITSTATUS(status));	
		}else if(WIFSIGNALED(status)){//被信号终止
			printf("child %d exit %d\n",pid,WTERMSIG(status));
		}
	}
}

int main(){
	int i = 0;
	pid_t pid;
	for(;i<10;i++){
		pid = fork();
		if(pid<0){
			print_err("fork error",-1);
		}else if(pid==0){
			break;
		}
	}
	if(pid==0){
		int n = 10;
		while(n){
			printf("child pid = %d\n",getpid());
			n--;
			sleep(1);
		}
	}else{
		struct sigaction act;
		act.sa_flags = 0;
		act.sa_handler = do_sig_child;
		sigemptyset(&act.sa_mask);
		sigaddset(&act.sa_mask,SIGQUIT);
		sigaction(SIGCHLD,&act,NULL);
		while(1){
			printf("parent pid = %d\n",getpid());
			sleep(1);
		}
	}
	return 0;
}

void print_err(const char * err,int exitno){
	perror(err);
	exit(exitno);
}

