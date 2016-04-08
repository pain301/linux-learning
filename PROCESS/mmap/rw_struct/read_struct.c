#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include "stu.h"

#define LEN 0x1000

void print_err(const char *,int);

int main(int argc,char **argv){
	if(argc<2){
		fprintf(stderr,"%s filename\n",argv[0]);
		exit(-1);
	}
	//以可读可写模式打开，否则mmap映射失败
	int fd = open(argv[1],O_RDWR);
	if(fd<0){
		print_err("open error",-2);
	}
	struct Stu *mm = mmap(NULL,LEN,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
	if(mm==MAP_FAILED){
		print_err("mmap error",-3);
	}
	unlink(argv[1]);
	while(1){
		printf("stu name: %s age = %d sex = %c\n",mm->name,mm->age,mm->sex);
		sleep(1);
	}
	munmap(mm,LEN);
	return 0;
}

void print_err(const char *err,int exitno){
	perror(err);
	exit(exitno);
}
