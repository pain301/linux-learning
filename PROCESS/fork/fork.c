#include <stdlib.h>
#include <stdio.h>

int main(){
	int pid;
	int ppid = getpid();
	int i = 0;
	for(;i<10;i++){
		if(getpid()==ppid){
			pid = fork();
		}else{
			break;	
		}
	}
	if(pid==0){
		printf("child process pid = %d\n",getpid());
	}else if(pid>0){
		printf("parent process pid = %d\n",getpid());
		sleep(1);
	}
	return 0;
}
