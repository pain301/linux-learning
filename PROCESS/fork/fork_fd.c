#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

int main(){
	char buf[1024] = {0};
	int fd = open("hello",O_RDONLY);
	if(fd==-1){
		perror("open error");
		exit(-1);
	}
	pid_t pid = fork();
	if(pid==0){
		sleep(3);
		//子进程从父进程继承来的文件描述符
		int ret = read(fd,buf,1024);
		if(ret==-1){
			perror("read fail");
			exit(-2);
		}
		printf("read by child process %s\n",buf);
	}else if(pid>0){
		close(fd);	
		printf("parent process has close fd\n");
		pid_t pid_c = wait(NULL);
		printf("parent process wait for %d\n",pid_c);
	}else{
		perror("fork error");
		exit(-1);
	}
	return 0;
}
