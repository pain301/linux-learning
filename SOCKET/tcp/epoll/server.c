#include "tcp_socket.h"

#define PORT 8000
#define OPEN_MAX 1024

int main(){
	struct sockaddr_in serveraddr,clientaddr;
	int addrlen;
	int listenfd;
	
	listenfd = Socket(AF_INET,SOCK_STREAM,0);
	
	bzero(&serveraddr,sizeof(struct sockaddr_in));
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
	
	int efd;
	struct epoll_event tep,ep[OPEN_MAX];	
	
	efd = epoll_create(OPEN_MAX);
	if(efd==-1){
		close(listenfd);
		handle_err("epoll_create error\n");
	}
	
	tep.events = EPOLLIN;
	tep.data.fd = listenfd;
	int ret = epoll_ctl(efd,EPOLL_CTL_ADD,listenfd,&tep); 
	if(ret==-1){
		close(listenfd);
		close(efd);
		handle_err("epoll_ctl error\n");
	}
	
	int readyfdNum;
	int tmpfd;
	char buf[1024] = {0};
	int i;
	while(1){
		//阻塞等待
		readyfdNum = epoll_wait(efd,ep,OPEN_MAX,-1);	
		if(readyfdNum==-1){
			close(listenfd);
			close(efd);
			handle_err("epoll_wait error\n");
		}
		for(i = 0;i<readyfdNum;++i){
			if(!(ep[i].events & EPOLLIN)){
				continue;
			}
			//产生新的连接
			if(ep[i].data.fd==listenfd){
				addrlen = sizeof(struct sockaddr_in);
				tmpfd = Accept(listenfd,(struct sockaddr *)&clientaddr,&addrlen);
				printf("%s:%d connect to server...\n",inet_ntoa(clientaddr.sin_addr),ntohs(clientaddr.sin_port));
				tep.events = EPOLLIN;
				tep.data.fd = tmpfd;
				ret = epoll_ctl(efd,EPOLL_CTL_ADD,tmpfd,&tep);
				if(ret==-1){
					printf("connect error\n");	
				}		
			}else{
				tmpfd = ep[i].data.fd;
				ret = Read(tmpfd,buf,sizeof(buf));
				if(ret<=0){
					epoll_ctl(efd,EPOLL_CTL_DEL,tmpfd,NULL);
					close(tmpfd);
					printf("client close...\n");	
					continue;
				}	
				int j = 0;
				for(;j<ret;++j){
					buf[j] = toupper(buf[j]);
				}
				Write(tmpfd,buf,ret);
				memset(buf,0,sizeof(buf));
			}
		}
	}
	close(listenfd);
	close(efd);	
	return 0;
}
