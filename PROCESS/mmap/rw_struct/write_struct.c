#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include "stu.h"

#define LEN 0x1000

void print_err(const char *,int );


int main(int argc,char **argv){
	if(argc<2){
		fprintf(stderr,"%s filename\n",argv[0]);
		exit(-1);
	}
	int fd = open(argv[1],O_RDWR|O_CREAT,0644);
	if(fd<0){
		print_err("open error",-1);
	}
	if((lseek(fd,LEN-1,SEEK_SET))<0){
		print_err("lseek error",-2);
	}
	if((write(fd,"\0",1))<0){
		print_err("write error",-3);
	}
	struct Stu *mm = mmap(NULL,LEN,PROT_WRITE|PROT_READ,MAP_SHARED,fd,0);
	if(mm==MAP_FAILED){
		print_err("mmap error",-4);
	}
	close(fd);
	int i = 0;
	while(1){
		mm->age = 20+i;
		sprintf(mm->name,"jack_%d",i);
		if(i%2==0)
			mm->sex = 'W';
		else
			mm->sex = 'M';
		i++;
		sleep(1);
	}
	munmap(mm,LEN);
	return 0;
}

void print_err(const char *str,int exitno){
	perror(str);
	exit(exitno);
}
