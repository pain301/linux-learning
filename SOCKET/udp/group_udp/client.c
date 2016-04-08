#include "udp_socket.h"

#define CLIENT_PORT 9000
#define SERVER_PORT 8000
#define GROUP_IP "239.0.2.2"

int main(){
	struct sockaddr_in serveraddr,clientaddr;
	int sockfd;

	sockfd = Socket(AF_INET,SOCK_DGRAM,0);
	
	bzero(&clientaddr,sizeof(struct sockaddr_in));
	clientaddr.sin_family = AF_INET;
	//clientaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	inet_pton(AF_INET,"0.0.0.0",&clientaddr.sin_addr.s_addr);
	clientaddr.sin_port = htons(CLIENT_PORT);
	
	Bind(sockfd,(struct sockaddr *)&clientaddr,sizeof(struct sockaddr_in));

	//设置组地址
	struct ip_mreqn groupaddr;
	//groupaddr.imr_multiaddr.s_addr = inet_addr(GROUP_IP);
	inet_pton(AF_INET,GROUP_IP,&groupaddr.imr_multiaddr);
	//groupaddr.imr_address.s_addr = htonl(INADDR_ANY);
	inet_pton(AF_INET,"0.0.0.0",&groupaddr.imr_address);
	groupaddr.imr_ifindex = if_nametoindex("eth0");	
	
	//client加入多播组
	setsockopt(sockfd,IPPROTO_IP,IP_ADD_MEMBERSHIP,&groupaddr,sizeof(struct ip_mreqn));
	
	int len;
	char buf[1024] = {0};
	while(1){
		recvfrom(sockfd,buf,sizeof(buf),0,NULL,NULL);	
		printf("receive: %s",buf);
	}
	close(sockfd);
	return 0;
}
