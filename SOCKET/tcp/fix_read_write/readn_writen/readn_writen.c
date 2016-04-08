#include "tcp_socket.h"

ssize_t readn(int fd,void *buf,size_t count){
	size_t leftCount = count;	
	size_t readCount;
	char *p = (char *)buf;
	while(leftCount>0){
		if((readCount = read(fd,p,leftCount))<0){
			if(errno==EINTR){
				continue;
			}
			return -1;
		}else if(readCount==0){
			return count-leftCount;
		}
		p+=readCount;
		leftCount-=readCount;
	}
	return count;
}

ssize_t writen(int fd,void *buf,size_t count){
	size_t leftCount = count;
	size_t writeCount;
	char *p = (char *)buf;
	while(leftCount>0){
		if((writeCount=write(fd,p,leftCount))<0){
			if(errno==EINTR){
				continue;
			}
			return -1;
		}else if(writeCount==0){
			return count-leftCount;
		}
		p+=writeCount;
		leftCount-=writeCount;
	}
	return count;
}
