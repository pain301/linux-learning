#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/mman.h>

int main(){
	int *p;
	int fd = open("hello",O_RDWR);
	if(fd<0){
		perror("open failed");
		exit(-1);
	}
	//获取文件大小
	int len = lseek(fd,0,SEEK_END);
	//可读可写权限
	p = mmap(NULL,len,PROT_WRITE|PROT_READ,MAP_SHARED,fd,0);
	//内存映射失败
	if(p==MAP_FAILED){
		perror("mmap failed");
		exit(-2);
	}
	close(fd);
	p[0] = 0x41424344;
	//释放内存映射
	munmap(p,len);
	return 0;
}
