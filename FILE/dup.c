#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

int main(){
	int fd = open("hello",O_RDWR|O_CREAT,0644);
	if(fd==-1){
		perror("open failed");
		exit(-1);
	}
	//？？
	printf("\n");
	close(1);
	int copy_fd = dup(fd);
	close(fd);
	printf("hello world\n");
	close(copy_fd);
	return 0;
}
