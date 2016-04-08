#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#define TRY_AGAIN "TRY_AGIAN\n"
#define TIME_OUT "TIME OUT\n"

int main(){
	int flags;
	//获取文件的访问控制属性
	flags = fcntl(STDIN_FILENO,F_GETFL);
	if(flags<0){
		perror("fcntl error");
		exit(-1);
	}
	//设置文件的访问控制属性
	flags |= O_NONBLOCK;
	//设置文件访问控制属性为非阻塞
	if(fcntl(STDIN_FILENO,F_SETFL,flags)==-1){
		perror("fcntl set error");
		exit(-2);
	}
	int i = 0;
	int len;
	char buf[1024];
	while(i<5){
		len = read(STDIN_FILENO,buf,sizeof(buf));	
		if(len>=0)
			break;
		if(errno!=EAGAIN){
			perror("read failed");
			exit(-3);
		}
		write(STDOUT_FILENO,TRY_AGAIN,sizeof(TRY_AGAIN));
		i++;
		sleep(1);
	}
	if(i==5){
		write(STDOUT_FILENO,TIME_OUT,sizeof(TIME_OUT));
	}else{
		write(STDOUT_FILENO,buf,len);
	}
	return 0;
}
