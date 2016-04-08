//read与write必须包含文件
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
//perror
#include <stdio.h>

int main(){
	char buf[10];
	int len = 0;
	while((len = read(STDIN_FILENO,buf,10))>0){
		write(STDOUT_FILENO,buf,strlen(buf));
	}
	if(len<0){
		perror("read failed");
		exit(-1);
	}
	return 0;
}
