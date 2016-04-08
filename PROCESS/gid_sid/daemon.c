#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

void daemon_(){
	pid_t pid;
	if((pid=fork())<0){
		perror("fork error");
		exit(-1);
	}else if(pid==0){
		//创建新的会话
		setsid();
		//改变当前工作目录
		if(chdir("/")<0){
			perror("chdir error");
			exit(-2);
		}
		//设置文件权限掩码
		umask(0);
		//关闭从父进程继承过来的文件描述符
		close(0);
		open("dev/null",O_RDWR);
		dup2(0,1);
		dup2(0,2);
	}else{
		exit(0);
	}
}

int main(){
	daemon_();
	while(1);
}
