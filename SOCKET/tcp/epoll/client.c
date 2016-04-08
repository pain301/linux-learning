#include "tcp_socket.h"

#define IP "192.168.150.133"
#define PORT 8000

int main(){
	struct sockaddr_in serveraddr;
	int connfd;
	
	connfd = Socket(AF_INET,SOCK_STREAM,0);
	bzero(&serveraddr,sizeof(struct sockaddr_in));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(IP);
	serveraddr.sin_port = htons(PORT);

	Connect(connfd,(struct sockaddr *)&serveraddr,sizeof(struct sockaddr_in));

	char buf[1024] = {0};
	int buflen ;
	while(fgets(buf,sizeof(buf),stdin)!=NULL){
		Write(connfd,buf,strlen(buf));
		buflen = Read(connfd,buf,sizeof(buf));
		if(buflen==0){
			break;
		}
		fputs(buf,stdout);
	}
	close(connfd);
	return 0;	
}
