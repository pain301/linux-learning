#include "tcp_socket.h"
#define PORT 8000

int main(){
	struct sockaddr_in serveraddr,clientaddr;
	int addrlen;
	int listenfd;
	int connfd;

	listenfd = Socket(AF_INET,SOCK_STREAM,0);

	bzero(&serveraddr,sizeof(struct sockaddr_in));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(PORT);

	int optval = 1;
	setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&optval,sizeof(optval));

	Bind(listenfd,(struct sockaddr_in *)&serveraddr,sizeof(struct sockaddr_in));

	Listen(listenfd,128);

	addrlen = sizeof(struct sockaddr_in);
	connfd = Accept(listenfd,(struct sockaddr_in *)&clientaddr,&addrlen);

	pid_t pid = fork();
	if(pid==0){
		signal(SIGUSR1,handle_sig);
		char send_buf[1024];
		int n;
		memset(send_buf,0,sizeof(send_buf));
		while(fgets(send_buf,sizeof(send_buf),stdin)!=NULL){
			n = strlen(send_buf);
			Writen(connfd,send_buf,n);
			memset(send_buf,0,sizeof(send_buf));
		}		
	}else if(pid>0){
		char recv_buf[1024]; 
		memset(recv_buf,0,sizeof(recv_buf));
		while(1){
			int ret = Readline(connfd,recv_buf,sizeof(recv_buf));
			if(ret<=0)
				break;
			fputs(recv_buf,stdout);
			memset(recv_buf,0,sizeof(recv_buf));
		}
		close(connfd);
		close(listenfd);
		kill(pid,SIGUSR1);	
		handle_err("readn error\n");
	}else{
		close(connfd);
		close(listenfd);
		handle_err("fork error\n");
	}

	return 0;
}
