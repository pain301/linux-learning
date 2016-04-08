#include <stdio.h>
#include <stdlib.h>

int main(){
	int pid = fork();
	if(pid==0){
		printf("child process pid = %d ppid = %d\n",getpid(),getppid());
		sleep(10);
	}else if(pid>0){
		while(1){
			//没有则返回-1
			//有子进程则返回回收的子进程pid
			int pid_c = wait(NULL);
			printf("parent pcocess pid = %d\n",getpid());
			printf("wait for child process pid = %d\n",pid_c);
			sleep(1);
		}
	}else{
		perror("fork error");
		exit(-1);
	}
	return 0;
}
