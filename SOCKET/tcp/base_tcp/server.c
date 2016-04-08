#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>

int main(){

	char buf[32] = {0};
	char content[1024];
	int sockfd = socket(AF_INET,SOCK_STREAM,0);
	if(sockfd<0){
		perror("socket error");
		exit(-1);
	}
	struct sockaddr_in serveraddr;
	//清空
	bzero(&serveraddr,sizeof(struct sockaddr_in));
	//IPV4
	serveraddr.sin_family = AF_INET;
	//IP地址
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	//端口号
	serveraddr.sin_port = htons(8000);
	//绑定
	int ret = bind(sockfd,(struct sockaddr *)&serveraddr,sizeof(struct sockaddr_in));
	if(ret<0){
		perror("bind error");
		exit(-2);
	}
	//最大等待排队数量128
	ret = listen(sockfd,128);
	if(ret<0){
		perror("listen error");
		exit(-3);
	}
	int connfd;
	struct sockaddr_in clientaddr;
	int addrlen;
	int len;
	while(1){
		addrlen = sizeof(struct sockaddr_in);
		connfd = accept(sockfd,(struct sockaddr *)&clientaddr,&addrlen);
		if(connfd<0){
			perror("accept error");
			exit(-4);
		}
		inet_ntop(AF_INET,&clientaddr.sin_addr.s_addr,buf,sizeof(buf));
		printf("ip: %s port: %d\n",buf,ntohs(clientaddr.sin_port));
		//处理客户端数据
		//不断读取，直到客户端关闭
		while(1){	
			len = read(connfd,content,sizeof(content));
			if(len<0){
				perror("read error");
				exit(-1);
			}else if(len==0){
				printf("客户端已经关闭\n");
				break;
			}
			int i = 0;
			while(i<len){
				content[i] = toupper(content[i]);
				i++;
			}
			write(connfd,content,len);
		}
		close(connfd);
	}
	close(sockfd);
	return 0;
}
