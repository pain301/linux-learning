#include "tcp_socket.h"
#define SERVERPORT 8000

struct client_info{
	struct sockaddr_in addr;
	int connfd;
};

void *work(void *arg);

int main(){
	struct sockaddr_in serveraddr;
	struct sockaddr_in clientaddr;
	int addrlen;
	int listenfd;
	int connfd;
	
	listenfd = Socket(AF_INET,SOCK_STREAM,0);
	
	bzero(&serveraddr,sizeof(struct sockaddr_in));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY); 
	serveraddr.sin_port = htons(SERVERPORT);
		
	int optval = 1;
	if((setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&optval,sizeof(optval)))==-1){
		handle_err("setsockopt error\n");
	}
	
	Bind(listenfd,(struct sockaddr *)&serveraddr,sizeof(struct sockaddr_in));
	Listen(listenfd,128);
	
	printf("Server start...\n");
	
	struct client_info clientInfo[300];
	int client_num = 0;
	pthread_t tid;
	while(1){
		addrlen = sizeof(struct sockaddr_in);
		connfd = Accept(listenfd,(struct sockaddr *)&clientaddr,&addrlen);
		clientInfo[client_num].addr = clientaddr;
		clientInfo[client_num].connfd = connfd;
		pthread_create(&tid,NULL,work,(void *)&clientInfo[client_num]);
		++client_num;
	}
	close(listenfd);
	return 0;
}

void* work(void *arg){
	struct client_info *info = (struct client_info *)arg;
	char buf[1024];
	pthread_detach(pthread_self());
	printf("work thread\n");
	int n;
	while(1){
		n = Read(info->connfd,buf,sizeof(buf));
		if(n<=0){
			break;
		}
		printf("receive data from %s:%d : %s",inet_ntoa(info->addr.sin_addr), htons(info->addr.sin_port),buf);
		int i;
		for(i=0;i<n;++i){
			buf[i] = toupper(buf[i]);
		}
		Write(info->connfd,buf,n);
	}
	Close(info->connfd);
}
