#include "tcp_socket.h"
#define IP "192.168.150.133"
#define PORT 8000

int main(){
	struct sockaddr_in serveraddr;
	int addrlen;
	int connfd;

	fd_set readset,tmpreadset;
	fd_set writeset,tmpwriteset;

	int readyfdNum;

	connfd = Socket(AF_INET,SOCK_STREAM,0);
	
	bzero(&serveraddr,sizeof(struct sockaddr_in));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(IP);
	serveraddr.sin_port = htons(PORT);

	Connect(connfd,(struct sockadd *)&serveraddr,sizeof(struct sockaddr_in));

	FD_ZERO(&readset);
	FD_SET(connfd,&readset);
	FD_SET(0,&readset);

	char recv_buf[10] = {0};
	char send_buf[1024] = {0};
	while(1){
		tmpreadset = readset;
		tmpwriteset = writeset;
		readyfdNum = select(connfd+1,&tmpreadset,NULL,NULL,NULL);
		if(readyfdNum==-1){
			close(connfd);
			handle_err("select error\n");
		}	
		//检测客户端是否关闭
		if(FD_ISSET(connfd,&tmpreadset)){
			printf("get read\n");
			break;
		}
		if(FD_ISSET(0,&tmpreadset)){
			if(fgets(send_buf,sizeof(send_buf),stdin)!=NULL)
				Write(connfd,send_buf,strlen(send_buf));
		}
	}
	#if 0
	while(fgets(send_buf,sizeof(send_buf),stdin)!=NULL){
		Write(connfd,send_buf,strlen(send_buf));
		memset(send_buf,0,sizeof(send_buf));
	}
	#endif
	close(connfd);
	return 0;
}
