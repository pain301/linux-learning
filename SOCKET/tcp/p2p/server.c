#include "tcp_socket.h"
#define PORT 8000

void handle_err(char *);
void handle_sig(int);

int main(){
	struct sockaddr_in serveraddr;
	struct sockaddr_in clientaddr;
	int sockfd;
	int connfd;
	int addrlen;

	sockfd = socket(AF_INET,SOCK_STREAM,0);
	if(sockfd<0){
		handle_err("socket error\n");		
	}

	bzero(&serveraddr,sizeof(struct sockaddr_in));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(PORT);

	int optval = 1;
	setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&optval,sizeof(optval));

	if(bind(sockfd,(struct sockaddr *)&serveraddr,sizeof(struct sockaddr_in))==-1){
		close(sockfd);
		handle_err("bind error\n");
	}

	if(listen(sockfd,128)==-1){
		close(sockfd);
		handle_err("listen error\n");
	}

	addrlen = sizeof(struct sockaddr);
	connfd = accept(sockfd,(struct sockaddr *)&clientaddr,&addrlen);
	if(connfd==-1){
		close(sockfd);
		handle_err("accept error\n");
	}
	printf("client %s connected\n",inet_ntoa(clientaddr.sin_addr));
	
	pid_t pid = fork();
	if(pid==0){
		signal(SIGUSR1,handle_sig);
		char send_buf[1024] = {0};
		while(fgets(send_buf,sizeof(send_buf),stdin)!=NULL){
			write(connfd,send_buf,strlen(send_buf));
			memset(send_buf,0,sizeof(send_buf));
		}
	}else if(pid>0){
		char recv_buf[1024] = {0};
		while(1){
			int len = read(connfd,recv_buf,sizeof(recv_buf));
			if(len==-1){
				close(sockfd);
				close(connfd);
				handle_err("read error\n");
			}else if(len==0){
				printf("client close\n");
				break;
			}else{
				fputs(recv_buf,stdout);
				memset(recv_buf,0,sizeof(recv_buf));
			}
		}
	}else{
		close(connfd);
		close(sockfd);
		handle_err("fork error\n");
	}
	kill(pid,SIGUSR1);
	close(connfd);
	close(sockfd);
	return 0;
}

void handle_err(char *err){
	perror(err);
	exit(-1);
}

void handle_sig(int num){
	printf("receive sig %d\n",num);
	exit(0);
}
