//非阻塞读取数据，限时
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define TRY_AGAIN "try again\n"
#define TIMEOUT "time out\n"

int main(){
	int fd = open("/dev/tty",O_RDONLY|O_NONBLOCK);
	if(fd<0){
		perror("open failed");
		exit(-1);
	}
	int i = 0;
	char buf[1024];
	int len1;
	while(i<5){
		len1 = read(fd,buf,sizeof(buf));
		if(len1>=0)
			break;
		if(errno!=EAGAIN){
			perror("read failed");
			exit(-2);	
		}
		write(STDOUT_FILENO,TRY_AGAIN,strlen(TRY_AGAIN));
		sleep(1);
		i++;
	}
	if(i==5){
		write(STDOUT_FILENO,TIMEOUT,strlen(TIMEOUT));
	}else{
		write(STDOUT_FILENO,buf,len1);
	}
	close(fd);
	return 0;
}
