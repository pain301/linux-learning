#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

void sys_err(const char *,int);

int main(int argc,char **argv){
	if(argc<2){
		fprintf(stderr,"%s fifoname\n",argv[0]);
		exit(-1);
	}
	char buf[20] = "hello fifo";
	int fd = open(argv[1],O_WRONLY);
	if(fd<0){
		sys_err("open failed",-2);
	}else{
		int ret = write(fd,buf,strlen(buf)+1);
		sleep(10);
		if(ret<0){
			sys_err("write failed",-3);
		}else if(ret==0){
			printf("nothing write\n");
		}
		printf("write %d\n",ret);
	}
}

void sys_err(const char *msg,int exitno){
	perror(msg);
	exit(exitno);
}
