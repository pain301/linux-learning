#include "tcp_socket.h"
#define SERVERIP "192.168.150.133"
#define PORT 8000

void handle_err(char *);
void handle_sig(int);

int main(){
	struct sockaddr_in serveraddr;
	int connfd;
	
	connfd = socket(AF_INET,SOCK_STREAM,0);	
	if(connfd==-1){
		handle_err("socket error\n");
	}
	
	bzero(&serveraddr,sizeof(struct sockaddr_in));	
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);
	serveraddr.sin_port = htons(PORT);

	if(connect(connfd,(struct sockaddr *)&serveraddr,sizeof(struct sockaddr_in))==-1){
		close(connfd);
		handle_err("connect error\n");
	}
	
	pid_t pid = fork();
		
	if(pid==0){
		struct packet recv_buf;
		memset(&recv_buf,0,sizeof(recv_buf));
		int n;
		while(1){
			int len = read(connfd,&recv_buf.len,4);
			if(len==-1){
				break;
			}else if(len<4){
				printf("server closed\n");
				break;
			}else{
				n = ntohl(recv_buf.len);
				len = readn(connfd,recv_buf.buf,n);
				if(len==-1){
					break;
				}else if(len<n){
					printf("server closed\n");
					break;
				}else{
					fputs(recv_buf.buf,stdout);
					memset(&recv_buf,0,sizeof(struct packet));
				}
			}
		}
		close(connfd);
		kill(getppid(),SIGUSR1);
		handle_err("readn error\n");
	}else if(pid>0){
		signal(SIGUSR1,handle_sig);
		struct packet send_buf;
		int n;
		while(fgets(send_buf.buf,sizeof(send_buf.buf),stdin)!=NULL){
			n = strlen(send_buf.buf);
			send_buf.len = htonl(n);
			writen(connfd,&send_buf,n+4);
			memset(&send_buf,0,sizeof(send_buf));
		}
		close(connfd);
	}else{
		close(connfd);
		handle_err("fork error\n");
	}
	
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
