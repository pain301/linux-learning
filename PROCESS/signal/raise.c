#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

int main(){
	pid_t pid;
	pid = fork();	
	if(pid<0){
		perror("fork error");
		exit(-1);
	}
	if(pid==0){
		printf("child %d wait for signal\n",getpid());
		raise(SIGSTOP);
		printf("after child stop\n");
	}else if(pid>0){
		sleep(2);
		if((waitpid(pid,NULL,WNOHANG))==0){
			if((kill(pid,SIGKILL))==0){
				printf("parent kill child %d\n",pid);
			}
		}
		waitpid(pid,NULL,0);
	}
	printf("hello world\n");
	return 0;
}
