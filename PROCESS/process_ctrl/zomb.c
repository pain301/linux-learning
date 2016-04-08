#include <stdio.h>
#include <stdlib.h>
int main(){
	int pid;
	pid = fork();
	int i = 0;
	if(pid==0){
		//子进程先退出，而父进程不进行回收，形参僵尸进程
		printf("child process pid = %d ppid = %d\n",getpid(),getppid());
	}else if(pid>0){
		printf("parent process pid = %d\n",getpid());
		sleep(10);
	}else{
		perror("fork error");
		exit(-1);
	}
	return 0;
}
