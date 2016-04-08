#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

#include <stdio.h>

#define SIZE 1024

int main(int argc,char **argv){
	if(argc<3){
		printf("app src dst\n");
		exit(-1);
	}	
	int srcfd = open(argv[1],O_RDONLY);
	int destfd = open(argv[2],O_RDWR|O_CREAT|O_TRUNC,0644);
	if(srcfd<0||destfd<0){
		perror("open failed");
		exit(-2);
	}
	char buf[SIZE];
	int len1 = 0;
	int len2 = 0;
	while((len1 = read(srcfd,buf,SIZE))>0){
		len2 = write(destfd,buf,len1);
		if(len2<0){
			perror("write failed");
			exit(-3);
		}
	}
	if(len1<0){
		perror("read failed");
		exit(-3);
	}
	close(srcfd);
	close(destfd);
	return 0;
}
