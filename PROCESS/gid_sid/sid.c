#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(){
	pid_t pid;
	if((pid=fork())<0){
		perror("fork error");
		exit(-1);
	}else if(pid==0){
		printf("child pid = %d\n",getpid());
		printf("parent pid = %d\n",getppid());
		printf("group id of child = %d\n",getpgid(0));
		printf("session id of child = %d\n",getsid(0));
		printf("\n");
		sleep(20);
		//子进程非组长进程，可以调用
		setsid();
		//子进程加入新的会话，成为会话的首进程
		//同时成为新进程组的组长进程
		printf("child pid = %d\n",getpid());
		printf("parent pid = %d\n",getppid());
		printf("group id of child = %d\n",getpgid(0));
		printf("session id of child = %d\n",getsid(0));
		sleep(20);
	}
	return 0;
}
