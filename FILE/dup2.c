#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int main(){
	int fd;
	int save_fd;
	char str[] = "hello world\n";
	//指定USR有读写权限
	fd = open("hello",O_RDWR|O_CREAT,S_IRUSR|S_IWUSR);
	if(fd==-1){
		perror("open error");
		exit(-1);
	}
	//保存标准输出的文件描述符
	save_fd = dup(STDOUT_FILENO); 
	dup2(fd,STDOUT_FILENO);
	close(fd);
	write(STDOUT_FILENO,str,strlen(str));
	
	//让STDOUT_FILENO重新指向标准输入输出
	dup2(save_fd,STDOUT_FILENO);
	write(STDOUT_FILENO,str,strlen(str));
	close(save_fd);
	return 0;
}
