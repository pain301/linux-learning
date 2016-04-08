#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc,char **argv){
	if(argc<2){
		fprintf(stderr,"%s filename\n",argv[0]);
		exit(-1);
	}
	int fd = open(argv[1],O_RDONLY);
	if(fd<0){
		perror("open error");
		exit(-1);
	}
	dup2(fd,STDIN_FILENO);
	close(fd);
	execl("./upper","upper",NULL);
	//execl只有在出错的情况下才返回
	//若继续执行下面的语句，则execl必然出错
	perror("execl error");
	exit(1);
}
