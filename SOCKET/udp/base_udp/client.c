#include "udp_socket.h"

#define IP "192.168.150.133"
#define PORT 8000

int main(){
	struct sockaddr_in serveraddr;
	int sockfd;
	
	sockfd = Socket(AF_INET,SOCK_DGRAM,0);
	
	bzero(&serveraddr,sizeof(struct sockaddr_in));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(IP);
	serveraddr.sin_port = htons(PORT);

	char buf[1024] = {0};
	int len;
	
	while(fgets(buf,sizeof(buf),stdin)!=NULL){
		len = sendto(sockfd,buf,strlen(buf),0,(struct sockaddr *)&serveraddr,sizeof(struct sockaddr_in));	
		if(len==-1){
			printf("sendto error\n");
			continue;
		}
		len = recvfrom(sockfd,buf,sizeof(buf),0,NULL,0);
		if(len==-1){
			printf("recvfrom error\n");
			continue;
		}
		fputs(buf,stdout);
	}
	close(sockfd);	
	return 0;
}
