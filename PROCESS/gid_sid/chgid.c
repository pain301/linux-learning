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
		printf("group id of child = %d\n",getpgid(0));
		printf("\n");
		sleep(3);
		printf("group id of child changed to %d\n",getpgid(0));
	}else{
		sleep(1);
		setpgid(pid,pid);
		sleep(5);
		printf("parent pid = %d\n",getpid());
		printf("parent of parent pid = %d\n",getppid());
		printf("group id of parent = %d\n",getpgid(0));
		printf("\n");
		setpgid(getpid(),getppid());
		printf("group id of parent changed to  %d\n",getpgid(0));
	}
	return 0;
}
