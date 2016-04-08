#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc ,char **argv){
	char buf[1024];
	int len;
	int sockfd = socket(AF_INET,SOCK_STREAM,0);
	if(sockfd<0){
		perror("socket error");
		exit(-1);
	}
	//初始化服务器地址
	struct sockaddr_in serveraddr;
	bzero(&serveraddr,sizeof(struct sockaddr_in));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr("192.168.150.133");
	serveraddr.sin_port = htons(8000);
	
	//链接服务器
	int ret = connect(sockfd,(struct sockaddr *)&serveraddr,sizeof(struct sockaddr_in));
	if(ret<0){
		perror("connect error");
		exit(-1);
	}
	//不停的从终端读取数据
	while((fgets(buf,sizeof(buf),stdin))!=NULL){
		write(sockfd,buf,strlen(buf)+1);
		len = read(sockfd,buf,sizeof(buf));
		write(STDOUT_FILENO,buf,len);
	}	
	close(sockfd);
	return 0;
}
