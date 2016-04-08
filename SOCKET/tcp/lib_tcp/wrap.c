#include "tcp_socket.h"

void handle_err(char *err){
	perror(err);
	exit(-1);
}

void handle_sig(int num){
	int pid;
	//回收所有子进程
	while((pid = waitpid(-1,NULL,WNOHANG))>0){
		printf("receive pid %d\n",pid);
	}
}

int wait_read(int fd,unsigned int wait_secs){
	int ret = 0;
	if(wait_secs>0){
		fd_set readset;
		FD_ZERO(&readset);
		FD_SET(fd,&readset);
		struct timeval timeout;
		timeout.tv_sec = wait_secs;
		timeout.tv_usec = 0;

		do{
			ret = select(fd+1,&readset,NULL,NULL,&timeout);
		}while(ret<0&&errno==EINTR);

		if(ret==0){
			ret = -1;
			errno = ETIMEDOUT;
		}else if(ret==1){
			ret = 0;
		}else if(ret==-1){
			close(fd);
			handle_err("select error\n");
		}
	}
	return ret;
}

int wait_write(int fd,unsigned int wait_secs){
	int ret = 0;
	if(wait_secs>0){
		fd_set writeset;
		FD_ZERO(&writeset);
		FD_SET(fd,&writeset);
		struct timeval timeout;
		timeout.tv_sec = wait_secs;
		timeout.tv_usec = 0;
		do{
			ret = select(fd+1,NULL,&writeset,NULL,&timeout);
		}while(ret<0&&errno==EINTR);
		if(ret==0){
			ret = -1;
			errno = ETIMEDOUT;
		}else if(ret==1){
			ret = 0;
		}else if(ret==-1){
			close(fd);
			handle_err("select error\n");
		}
	}
	return ret;
}

int wait_accept(int fd,struct sockaddr_in *sa,unsigned int wait_secs){
	int ret;
	int addrlen = sizeof(struct sockaddr);
	if(wait_secs>0){
		fd_set acceptset;
		FD_ZERO(&acceptset);
		FD_SET(fd,&acceptset);
		struct timeval timeout;
		timeout.tv_sec = wait_secs;
		timeout.tv_usec = 0;
		ret = select(fd+1,NULL,&acceptset,NULL,&timeout);
		if(ret==-1){
			return -1;
		}else if(ret==0){
			errno = ETIMEDOUT;
			ret = -1;
		}
	}
	
	ret = Accept(fd,(struct sockaddr *)sa,&addrlen);	
	return ret;
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

void setNonBlock(int fd){
	int ret;
	int flags = fcntl(fd,F_GETFL);
	if(flags==-1)
		handle_err("fcntl error\n");
	flags|=O_NONBLOCK;
	ret = fcntl(fd,F_SETFL,flags);
	if(ret==-1)
		handle_err("fcntl error\n");
}

void setBlock(int fd){
	int ret;
	int flags = fcntl(fd,F_GETFL);
	if(flags==-1)
		handle_err("fcntl error\n");
	flags&=~O_NONBLOCK;
	ret = fcntl(fd,F_SETFL,flags);
	if(ret==-1)
		handle_err("fcntl error\n");
}

int wait_connect(int sockfd,struct sockaddr_in *sa,unsigned int wait_secs){
	int ret;
	int addrlen = sizeof(struct sockaddr_in);
	if(wait_secs>0)
		setNonBlock(sockfd);
	ret = connect(sockfd,(struct sockaddr *)sa,addrlen);
	if(ret<0&&errno==EINPROGRESS){
		fd_set connset;
		FD_ZERO(&connset);
		FD_SET(sockfd,&connset);
		struct timeval timeout;
		timeout.tv_sec = wait_secs;
		timeout.tv_usec = 0;
		do{
			ret = select(sockfd+1,NULL,&connset,NULL,&timeout);
		}while(ret<0&&errno==EINTR);	
		if(ret==0){
			ret = -1;
			errno = ETIMEDOUT;	
		}else if(ret<0)
			return -1;
		else if(ret==1){
			int err;
			socklen_t socklen = sizeof(err);
			int sockoptret = getsockopt(sockfd,SOL_SOCKET,SO_ERROR,&err,&socklen);
			if(sockoptret==-1)
				return -1;
			else if(sockoptret==0)
				ret = 0;
			else{
				errno = err;
				ret = -1;
			}
		}
	}	
	if(wait_secs>0)
		setBlock(sockfd);
	return ret;
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

typedef struct _TCPHandle{
	int sockfdArr[100];
	int arrNum;
	int sockfd;
	int conntime;
	int sendtime;
	int recvtime;
}TcpHandle;

/*客户端API*/

int client_init(void **handle,int conntime,int sendtime,int recvtime,int connNum){
	int ret = 0;
	if(handle==NULL||conntime<0||sendtime<0||recvtime<0){
		ret = TCP_ERRPARAM;
		printf("client_init %d\n",ret);
		return ret;
	}	
	
	TcpHandle *tmp = (TcpHandle *)malloc(sizeof(TcpHandle));
	if(tmp==NULL){
		ret = TCP_ERRMALLOC;
		printf("client_init %d\n",ret);
		return ret;
	}
	tmp->conntime = conntime;
	tmp->sendtime = sendtime;
	tmp->recvtime = recvtime;
	tmp->arrNum = connNum;
	
	*handle = tmp;
	return ret;
}

int client_getconn(void *handle,char *ip,char port,int *connfd){
	int ret = 0;
	TcpHandle *tmp = NULL;
	if(handle==NULL||ip==NULL||connfd==NULL||port<0||port>65536){
		ret = TCP_ERRPARAM;
		printf("client_getconn %d\n",ret);
		return ret;
	}
	int tmpfd = socket(AF_INET,SOCK_STREAM,0);
	if(tmpfd==-1){
		ret = errno;
		printf("client_getconn %d\n",ret);
		return ret;
	}
	
	struct sockaddr_in serveraddr;
	bzero(&serveraddr,sizeof(struct sockaddr_in));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(ip);
	serveraddr.sin_port = htons(port);

	tmp = handle;
	
	ret = wait_connect(tmpfd,&serveraddr,tmp->conntime);
	if(ret<0){
		if(ret==-1&&errno==ETIMEDOUT){
			ret = TCP_ERRTIMEOUT;
			return ret;
		}else{
			printf("client_getconn %d\n",ret);
		}
	}
	
	*connfd = tmpfd;
	return ret;
}

int client_send(void *handle,int connfd,unsigned char *data,int datalen){
	int ret;
	TcpHandle *tmp = handle;
	ret = wait_write(connfd,tmp->sendtime);
	if(ret==0){
		int writeNum = 0;
		char *tmpdata = malloc(datalen+4);
		if(tmpdata==NULL){
			ret = TCP_ERRMALLOC;
			printf("client_send %d\n",ret);
			return ret;
		}
		int netlen = htonl(datalen);
		memcpy(tmpdata,&netlen,4);
		memcpy(tmpdata+4,data,datalen);
		writeNum = Writen(connfd,tmpdata,datalen+4);
		free(tmpdata);
		if(writeNum<datalen+4){
			return writeNum;
		}	
	}	
	
	if(ret<0){
		if(ret==-1&&errno==ETIMEDOUT){
			ret = TCP_ERRTIMEOUT;
			return ret;
		}
		return ret;
	}

	return ret;
}

int client_recv(void *handle,int connfd,unsigned char *data,int *datalen){
	TcpHandle *tmp = handle;
	int ret;
	ret = wait_read(connfd,tmp->recvtime);		
	if(ret==0){
		int readNum = 0;
		int netlen;
		readNum = Readn(connfd,&netlen,4);			
		if(readNum<4){
			ret = -1
			return ret;
		}
		netlen = ntohl(netlen);
		if(netlen>*datalen){
			ret = TCP_ERRPARAM;
			return ret;
		}	
		readNum = Readn(connfd,data,netlen);
		*datalen = readNum;
		if(readNum<netlen){
			ret = -1;
			return ret;
		}
	}else if(ret<0){
		if(ret==-1&&errno==ETIMEDOUT){
			ret = ETIMEDOUT;
			return ret;
		}
	}
	return ret;
}

int client_close(int *connfd){
	return 0;
}

int client_destroy(void *handle){
	if(handle!=NULL){
		free(handle);
		handle = NULL;
	}
	return 0;
}
