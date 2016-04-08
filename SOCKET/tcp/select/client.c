#include "tcp_socket.h"

#define IP "192.168.150.133"
#define PORT 8000
#define MAXNUM 1024

int main(){
	struct sockaddr_in serveraddr;
	int connfd;
	
	char send_buf[MAXNUM];
	char recv_buf[MAXNUM];

	connfd = Socket(AF_INET,SOCK_STREAM,0);
	
	bzero(&serveraddr,sizeof(struct sockaddr_in));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(IP);
	serveraddr.sin_port = htons(PORT);

	Connect(connfd,(struct sockaddr *)&serveraddr,sizeof(struct sockaddr_in));
	
	while(fgets(send_buf,sizeof(send_buf),stdin)!=NULL){
		Write(connfd,send_buf,strlen(send_buf));
		memset(send_buf,0,sizeof(send_buf));
		int ret = Read(connfd,recv_buf,sizeof(recv_buf));
		if(ret<=0){
			printf("read error\n");
			break;
		}
		fputs(recv_buf,stdout);
		memset(recv_buf,0,sizeof(recv_buf));
	}
	close(connfd);
	return 0;
}
