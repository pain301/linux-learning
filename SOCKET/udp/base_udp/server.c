#include "udp_socket.h"

#define PORT 8000

int main(){
	struct sockaddr_in serveraddr,clientaddr;
	int addrlen;
	int sockfd;
	
	sockfd = Socket(AF_INET,SOCK_DGRAM,0);
	
	bzero(&serveraddr,sizeof(struct sockaddr_in));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(PORT);
	
	Bind(sockfd,(struct sockaddr *)&serveraddr,sizeof(struct sockaddr_in));
	
	char buf[1024] = {0};
	int len;
	int i;
	while(1){
		addrlen = sizeof(struct sockaddr_in);
		len = recvfrom(sockfd,buf,sizeof(buf),0,(struct sockaddr *)&clientaddr,&addrlen);	
		if(len==-1){
			printf("recvfrom error\n");
			continue;
		}
		printf("recvfrom %s:%d\n",inet_ntoa(clientaddr.sin_addr),ntohs(clientaddr.sin_port));
		for(i=0;i<len;++i){
			buf[i] = toupper(buf[i]);
		}
		len = sendto(sockfd,buf,len,0,(struct sockaddr *)&clientaddr,sizeof(struct sockaddr_in));
		if(len==-1)
			printf("sendto error\n");
	}
	return 0;
}
