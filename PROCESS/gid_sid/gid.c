#include <stdio.h>
#include <stdlib.h>

int main(){
	pid_t pid;	
	if((pid = fork())<0){
		perror("fork");
		exit(-1);
	}else if(pid==0){
		printf("child pid = %d\n",getpid());

		printf("group id = %d\n",getpgrp());
		printf("group id = %d\n",getpgid(0));
		printf("group id = %d\n",getpgid(getpid()));
	}else{
		sleep(3);
		printf("parent id = %d\n",getpid());
		printf("group id = %d\n",getpgrp());
	}
	return 0;
}
