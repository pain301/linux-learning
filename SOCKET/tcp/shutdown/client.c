#include "tcp_socket.h"

#define IP "192.168.150.133"
#define PORT 8000

int main(){
	struct sockaddr_in serveraddr;
	int connfd;
	
	char buf[1024] = {0};

	connfd = Socket(AF_INET,SOCK_STREAM,0);
	
	bzero(&serveraddr,sizeof(struct sockaddr_in));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(IP);
	serveraddr.sin_port = htons(PORT);

	Connect(connfd,(struct sockaddr_in *)&serveraddr,sizeof(struct sockaddr_in));

	while(fgets(buf,sizeof(buf),stdin)!=NULL){
		Write(connfd,buf,strlen(buf));
		int ret = Read(connfd,buf,sizeof(buf));
		if(ret==0){
			printf("server close\n");
			break;
		}
		memset(buf,0,sizeof(buf));
	}
	close(connfd);
	return 0;
}
