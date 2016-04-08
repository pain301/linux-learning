#include <stdio.h>
#include <stdlib.h>

int main(){
	pid_t pid;
	int n = 3;
	while(n--){
		pid = fork();
		if(pid==0){
			break;
		}else if(pid<0){
			perror("fork error");
			break;
		}	
	}
	if(pid==0){
		while(1){
			printf("child process pid = %d\n",getpid());
			sleep(3);
		}
	}else if(pid>0){
		pid_t pid_c;
		while(1){
			//非阻塞
			pid_c = waitpid(0,NULL,WNOHANG);
			if(pid_c==-1){
				printf("no child process\n");
				continue;
			}else if(pid_c>0){
				printf("wait for child pid = %d\n",pid_c);
			}
			sleep(1);
		}
	}
	return 0;
}
