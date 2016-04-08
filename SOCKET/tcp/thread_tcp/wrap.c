#include "tcp_socket.h"

void handle_err(char *err){
	perror(err);
	exit(-1);
}

void handle_sig(int num){
	printf("receive sig %d\n",num);
	exit(0);
}

ssize_t Read(int fd,void *buf,size_t count){
	ssize_t n;
again:
	if((n=read(fd,buf,count))==-1){
		if(errno==EINTR){
			goto again;
		}else
			return -1;
	}
	return n;
}

ssize_t Write(int fd,void *buf,size_t count){
	ssize_t n;
again:
	if((n=write(fd,buf,count))==-1){
		if(errno==EINTR){
			goto again;
		}else
			return -1;
	}
	return n;
}

ssize_t Readn(int fd,void *buf,size_t count){
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

ssize_t Writen(int fd,void *buf,size_t count){
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

ssize_t Peek(int fd,void *buf,size_t count){
	while(1){
		int ret = recv(fd,buf,count,MSG_PEEK);
		if(ret==-1&&errno==EINTR){
			continue;
		}
		return ret;
	}
}

ssize_t Readline(int fd,void *buf,size_t maxCount){
	int ret;
	int readCount;
	int leftCount = maxCount;
	char *p = buf;
	while(1){
		ret = Peek(fd,p,leftCount);
		if(ret<=0){
			return ret;
		}	
		int i;
		readCount = ret;
		for(i=0;i<readCount;++i){
			if(p[i]=='\n'){
				ret = Readn(fd,p,i+1);
				if(ret!=i+1){
					return -1;
				}
				return ret;	
			}
		}
		if(readCount==leftCount){
			return -1;
		}
		ret = Readn(fd,p,readCount);
		if(ret!=readCount){
			return -1;
		}
		p+=readCount;
		leftCount-=readCount;
	}	
	return -1;
}

int Socket(int family,int type,int protocol){
	int ret;
	if((ret = socket(family,type,protocol))==-1){
		handle_err("socket error\n");
	}
	return ret;
}

void Bind(int sockfd,const struct sockaddr *sa,socklen_t len){
	if((bind(sockfd,sa,len))==-1){
		close(sockfd);
		handle_err("bind error\n");
	}
}

void Listen(int sockfd,int backlog){
	if(listen(sockfd,backlog)==-1){
		close(sockfd);
		handle_err("listen error\n");
	}
}

int Accept(int sockfd,struct sockaddr *sa,socklen_t *len){
	int connfd;
again:
	if((connfd=accept(sockfd,sa,len))==-1){
		if(errno==ECONNABORTED||errno==EINTR){
			goto again;
		}else{
			close(sockfd);
			handle_err("accept error\n");
		}
	}
	return connfd;
}

void Connect(int sockfd,const struct sockaddr *sa,socklen_t len){
	if(connect(sockfd,sa,len)==-1){
		close(sockfd);
		handle_err("connect error\n");
	}
}

void Close(int fd){
	if(close(fd)==1){
		handle_err("close error\n");
	}
}
