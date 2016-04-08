#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void sys_err(const char *,int);

int main(int argc,char **argv){
	if(argc<2){
		fprintf(stderr,"%s fifoname\n",argv[0]);
		exit(-1);
	}
	char buf[20] = {0};	
	int fd = open(argv[1],O_RDONLY);
	if(fd<0){
		sys_err("open failed",-2);
	}else{
		sleep(3);
		int ret = read(fd,buf,sizeof(buf));
		if(ret<0){
			sys_err("read failed",-3);
		}
		ret = write(STDOUT_FILENO,buf,strlen(buf));
		if(ret<0){
			sys_err("write failed",-4);
		}
		printf("read %d\n",ret);
		close(fd);
	}
}

void sys_err(const char *msg,int exitno){
	perror(msg);
	exit(exitno);
}
