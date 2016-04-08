#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

#define MSG "wait to read\n"

int main(){
	int fd[2];
	char str[] = "Hello World\n";
	char str_c[1024] = {0};
	if(pipe(fd)<0){
		perror("pipe error");
		exit(-1);
	}
	pid_t pid;
	pid = fork();
	if(pid==0){
		int flags;
		int len;
		close(fd[1]);
		//非阻塞读取
		flags = fcntl(fd[0],F_GETFL);
		flags|=O_NONBLOCK;
		fcntl(fd[0],F_SETFL,flags);
		while((len = read(fd[0],str_c,1024))<0){
			if(errno==EAGAIN){
				write(STDOUT_FILENO,MSG,sizeof(MSG));	
				sleep(1);
				continue;
			}else{
				perror("read error");
				exit(-2);
			}
		}
		//读取之后关闭文件描述操作符
		close(fd[0]);
		sleep(3);
		write(STDOUT_FILENO,str_c,len);
	}else if(pid>0){
		close(fd[0]);
		sleep(2);
		write(fd[1],str,strlen(str));
		//写入数据之后关闭文件描述符，防止内存泄露
		close(fd[1]);
		pid_t pid_c = wait(NULL);
		fprintf(stdout,"wait for process %d\n",pid_c);
	}else{
		perror("fork error");
		exit(-2);
	}
	return 0;
}
