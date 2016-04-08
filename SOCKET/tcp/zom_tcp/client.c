#include "tcp_socket.h"
#define IP "192.168.150.133"
#define PORT 8000

int main(){
	struct sockaddr_in serveraddr;
	int sockfd[5];
	int addrlen;

	int i = 0;
	for(;i<5;++i){
		sockfd[i] = Socket(AF_INET,SOCK_STREAM,0);
		bzero(&serveraddr,sizeof(struct sockaddr_in));
		serveraddr.sin_family = AF_INET;
		serveraddr.sin_addr.s_addr = inet_addr(IP);
		serveraddr.sin_port = htons(8000);
		addrlen = sizeof(struct sockaddr_in);
		Connect(sockfd[i],(struct sockaddr *)&serveraddr,addrlen);
	}
	i = 0;
	char buf[1024] = {0};
	while(fgets(buf,sizeof(buf),stdin)!=NULL){
		Write(sockfd[i],buf,strlen(buf));
		memset(buf,0,sizeof(buf));
	}
	for(i=0;i<5;++i)
		Close(sockfd[i]);
	return 0;
}
