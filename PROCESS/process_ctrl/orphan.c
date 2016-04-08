#include <stdio.h>
#include <stdlib.h>

int main(){
	pid_t pid;
	pid = fork();
	//父进程先退出，子进程成为孤儿进程
	if(pid==0){
		printf("child process pid = %d ppid = %d\n",getpid(),getppid());
		sleep(10);
	}else if(pid>0){
		printf("parent pocess pid = %d\n",getpid());
	}else{
		perror("fork error");
		exit(-1);
	}
	return 0;
}
