//lseek需要使用的头文件
#include <sys/types.h>
#include <unistd.h>

#include <sys/stat.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>

int main(){
	int fd;
	fd = open("hello",O_WRONLY|O_CREAT,0644);
	if(fd<0){
		perror("open error");
		exit(-1);
	}
	//拓展文件
	lseek(fd,1024,SEEK_SET);
	write(fd,"a",1);
	//查看文件大小
	printf("filesize = %d\n",lseek(fd,0,SEEK_END));
	close(fd);
	return 0;
}
