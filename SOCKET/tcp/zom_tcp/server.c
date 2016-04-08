#include "tcp_socket.h"

#define PORT 8000

int main(){
	struct sockaddr_in serveraddr;
	struct sockaddr_in clientaddr;
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
	
	//忽略可以避免僵尸进程
	//signal(SIGCHLD,SIG_IGN);
	signal(SIGCHLD,handle_sig);

	while(1){
		addrlen = sizeof(struct sockaddr_in);
		int connfd = Accept(listenfd,(struct sockaddr *)&clientaddr,&addrlen);
		pid_t pid = fork();	
		if(pid==0){
			close(listenfd);
			char buf[1024] = {0};
			while(1){
				int ret = Read(connfd,buf,sizeof(buf));
				if(ret<=0){
					break;
				}
				fputs(buf,stdout);
				memset(buf,0,sizeof(buf));
			}	
			close(connfd);
			handle_err("read error\n");
		}else if(pid<0){
			break;
		}
	}
	handle_err("fork error\n");
	close(listenfd);
	return 0;
}
