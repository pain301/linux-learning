#include "udp_socket.h"

#define SERVER_PORT 8000
#define CLIENT_PORT 9000

#define GROUP_IP "239.0.2.2"

int main(){
	struct sockaddr_in serveraddr,clientaddr;
	socklen_t addrlen;
	int sockfd;
	
	sockfd = Socket(AF_INET,SOCK_DGRAM,0);
	
	bzero(&serveraddr,sizeof(struct sockaddr_in));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVER_PORT);
	
	Bind(sockfd,(struct sockaddr *)&serveraddr,sizeof(struct sockaddr_in));

	struct ip_mreqn groupaddr;
	
	//设置组地址
	//groupaddr.imr_multiaddr.s_addr = inet_addr(GROUP_IP);
	inet_pton(AF_INET,GROUP_IP,&groupaddr.imr_multiaddr);
	//groupaddr.imr_address.s_addr = htonl(INADDR_ANY);
	inet_pton(AF_INET,"0.0.0.0",&groupaddr.imr_address);
	groupaddr.imr_ifindex = if_nametoindex("eth0");
	
	setsockopt(sockfd,IPPROTO_IP,IP_MULTICAST_IF,&groupaddr,sizeof(struct ip_mreqn));	
	
	bzero(&clientaddr,sizeof(struct sockaddr_in));
	clientaddr.sin_family = AF_INET;
	clientaddr.sin_addr.s_addr = inet_addr(GROUP_IP);
	clientaddr.sin_port = htons(CLIENT_PORT);
	
	char buf[1024] = {0};
	while(fgets(buf,sizeof(buf),stdin)!=NULL){
		printf("%s %d",buf,strlen(buf));
		int ret = sendto(sockfd,buf,strlen(buf),0,(struct sockaddr*)&clientaddr,sizeof(struct sockaddr));	
		if(ret<0)
			perror("sendto error");
		memset(buf,0,sizeof(buf));
	}
	
	Close(sockfd);
	return 0;
}
