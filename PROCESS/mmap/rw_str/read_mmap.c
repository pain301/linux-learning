#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>

#define LEN 0x1000

void print_err(const char *,int);

int main(int argc,char **argv){
	if(argc<2){
		fprintf(stderr,"%s filename\n",argv[0]);
		exit(-1);
	}
	int fd = open(argv[1],O_RDWR);
	if(fd<0){
		print_err("open failed",-2);
	}
	char *mm = mmap(NULL,LEN,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
	if(mm==MAP_FAILED){
		print_err("mmap failed",-3);
	}
	close(fd);
	//删除磁盘文件
	unlink(argv[1]);

	while(1){
		printf("%s",mm);
		sleep(1);
	}
	munmap(mm,LEN);
	return 0;
}

void print_err(const char *err,int exitno){
	perror(err);
	exit(exitno);
}
