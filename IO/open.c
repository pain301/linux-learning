//open必须包含之头文件
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(void){
	int fd;
	//创建文件时修改掩码
	umask(0);
	fd = open("hello",O_CREAT|O_RDONLY,0777);
	if(fd<0){
		perror("open failed");
	}
	return 0;
}
