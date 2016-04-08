//非阻塞读取数据
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define TRY_AGAIN "try again\n"

int main(){
	int fd;
	fd = open("/dev/tty",O_RDONLY|O_NONBLOCK);
	if(fd<0){
		perror("open failed");
		exit(-1);
	}
	int len1;
	int len2;
	char buf[1024];
	while((len1 = read(fd,buf,sizeof(buf)))<0){
		if(errno!=EAGAIN){
			perror("read failed");
			exit(-2);
		}
		sleep(1);
		printf("%s",TRY_AGAIN);
	}
	len2 = write(STDOUT_FILENO,buf,len1);
	if(len2<0){
		perror("write error");
		exit(-2);
	}
	close(fd);
	return 0;
}
