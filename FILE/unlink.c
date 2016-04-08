#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc,char **argv){
	if(argc<2){
		fprintf(stderr,"%s <filename>\n",argv[0]);
		exit(0);
	}
	int fd = open(argv[1],O_RDWR|O_CREAT,0644);
	if(fd<0){
		perror("open error");
		exit(-1);
	}
	int ret = unlink(argv[1]);
	if(ret==-1){
		perror("unlink error");
		exit(-2);
	}
	return 0;
}
