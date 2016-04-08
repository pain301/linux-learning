#include "tcp_socket.h"

#define PORT 8000
#define OPEN_MAX 1024

int main(){
	struct sockaddr_in serveraddr,clientaddr;
	int addrlen;

	int listenfd;
	int tmpfd;
	
	listenfd = Socket(AF_INET,SOCK_STREAM,0);
	
	bzero(&serveraddr,sizeof(struct sockaddr_in));
	//bzero(&serveraddr,0);
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(PORT);	
	
	int optval = 1;
	if(setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&optval,sizeof(optval))==-1){
		close(listenfd);
		handle_err("setsockopt error\n");
	}
	Bind(listenfd,(struct sockaddr *)&serveraddr,sizeof(struct sockaddr_in));
	Listen(listenfd,128);
	
	struct pollfd connfd[OPEN_MAX];		

	connfd[0].fd = listenfd;
	//监听普通读事件
	connfd[0].events = POLLRDNORM;
	int i;
	for(i=1;i<OPEN_MAX;++i){
		connfd[i].fd = -1;
	}
	int maxIndex = 0;
	int readyfdNum;	

	char buf[1024] = {0};
	int buflen;

	while(1){
		//阻塞式
		readyfdNum = poll(connfd,maxIndex+1,-1);	
		//客户端连接请求
		if(connfd[0].revents & POLLRDNORM){
			addrlen = sizeof(struct sockaddr_in);
			tmpfd = Accept(listenfd,(struct sockaddr *)&clientaddr,&addrlen);		
			for(i=1;i<OPEN_MAX;++i){
				if(connfd[i].fd==-1){
					connfd[i].fd = tmpfd;
					break;
				}
			}
			if(i==OPEN_MAX){
				printf("too many conn\n");
				continue;
			}
			connfd[i].events = POLLRDNORM;
			if(i>maxIndex)
				maxIndex = i;
			if(--readyfdNum==0){
				continue;
			}
		}	
		for(i=1;i<=maxIndex;++i){
			if(connfd[i].fd==-1)
				continue;
			tmpfd = connfd[i].fd;
			if(connfd[i].revents & POLLRDNORM){
				buflen = Read(tmpfd,buf,sizeof(buf));
				//fuck !!!
				//if(buflen=Read(tmpfd,buf,sizeof(buf))<0){
				if(buflen<0){
					close(connfd[i].fd);
					connfd[i].fd = -1;
					handle_err("read error\n");
				}else if(buflen==0){
					close(tmpfd);
					connfd[i].fd = -1;
				}else{
					int j = 0;
					for(;j<buflen;++j){
						buf[j] = toupper(buf[j]);
					}
					Write(connfd[i].fd,buf,buflen);
					memset(buf,0,sizeof(buf));
				}			
				if(--readyfdNum==0)
					break;
			}
		}
	}	
	return 0;
}
