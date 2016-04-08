#include "tcp_socket.h"
#define SERVERIP "192.168.150.133"
#define SERVERPORT 8000

int main(){
	struct sockaddr_in serveraddr;
	int connfd;
	
	connfd = Socket(AF_INET,SOCK_STREAM,0);
	
	bzero(&serveraddr,sizeof(struct sockaddr_in));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);
	serveraddr.sin_port = htons(SERVERPORT);

	Connect(connfd,(struct sockaddr *)&serveraddr,sizeof(struct sockaddr_in));

	char buf[1024];

	while(fgets(buf,sizeof(buf),stdin)!=NULL){
		Write(connfd,buf,strlen(buf));
		int n = Read(connfd,buf,sizeof(buf));
		if(n<=0){
			printf("server closed\n");
			break;
		}
		Write(STDOUT_FILENO,buf,n);
	}
	Close(connfd);
	return 0;
}
