#include "tcp_socket.h"
#define SERVERIP "192.168.150.133"
#define PORT 8000

void handle_err(char *);
void handle_sig(int);

int main(){
	struct sockaddr_in clientaddr;
	int sockfd;
	sockfd = socket(AF_INET,SOCK_STREAM,0);
	if(sockfd==-1){
		handle_err("socket error\n");
	}
	bzero(&clientaddr,sizeof(struct sockaddr_in));
	clientaddr.sin_family = AF_INET;
	clientaddr.sin_addr.s_addr = inet_addr(SERVERIP);	
	clientaddr.sin_port = htons(PORT);

	if((connect(sockfd,(struct sockaddr *)&clientaddr,sizeof(struct sockaddr_in)))==-1){
		close(sockfd);
		handle_err("connect error");
	}

	pid_t pid = fork();

	//父进程发送数据
	if(pid>0){
		char send_buf[1024] = {0};
		while(fgets(send_buf,sizeof(send_buf),stdin)!=NULL){
			write(sockfd,send_buf,strlen(send_buf));
			memset(send_buf,0,sizeof(send_buf));
		}
	}else if(pid==0){
		char recv_buf[1024] = {0};
		while(1){
			int len = read(sockfd,recv_buf,sizeof(recv_buf));
			if(len==-1){
				handle_err("read error\n");
			}else if(len==0){
				printf("server close\n");
				break;
			}else{
				fputs(recv_buf,stdout);
				memset(recv_buf,0,sizeof(recv_buf));
			}
		}
		close(sockfd);
		kill(getppid(),SIGUSR1);
	}else{
		close(sockfd);
		handle_err("fork error\n");
	}

	close(sockfd);
}

void handle_err(char *err){
	perror(err);
	exit(-1);
}

void handle_sig(int num){
	printf("receive sig %d\n",num);
	exit(0);
}
