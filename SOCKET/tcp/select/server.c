#include "tcp_socket.h"

#define PORT 8000
#define MAXNUM 1024

int main(){
	struct sockaddr_in serveraddr,clientaddr;
	int addrlen;

	int listenfd, maxfd, tmpfd;
	int connfd[FD_SETSIZE];
	int maxIndex;

	fd_set fdset,tmpset;	
	
	int readyfdNum;

	char buf[MAXNUM] = {0};

	listenfd = Socket(AF_INET,SOCK_STREAM,0);
	
	bzero(&serveraddr,0);
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(PORT);
	
	int optval = 1;
	if(setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&optval,sizeof(optval))==-1){
		close(listenfd);
		handle_err("setsockopt error\n");
	}
	Bind(listenfd,(struct sockaddr *)&serveraddr,sizeof(struct sockaddr_in));
	Listen(listenfd,128);
	
	maxIndex = -1;
	maxfd = listenfd;
	int i = 0;
	
	//初始化与客户端通信的fd
	for(;i<FD_SETSIZE;++i){
		connfd[i] = -1;
	}
	//初始化fd集合
	FD_ZERO(&fdset);
	FD_SET(listenfd,&fdset);
	while(1){
		tmpset = fdset;	
		readyfdNum = select(maxfd+1,&tmpset,NULL,NULL,NULL);
		if(readyfdNum<0){
			close(listenfd);
			handle_err("select error\n");
		}
		
		//判断是否有新的客户端连接
		if(FD_ISSET(listenfd,&tmpset)){
			addrlen = sizeof(struct sockaddr_in);
			tmpfd = Accept(listenfd,(struct sockaddr *)&clientaddr,&addrlen);				
			printf("%s:%d connect to server...\n",
				inet_ntoa(clientaddr.sin_addr),
				htons(clientaddr.sin_port));		
	
			//将新连接到的fd添加到connfd数组中
			for(i=0;i<FD_SETSIZE;++i){
				if(connfd[i]<0){
					connfd[i] = tmpfd;
					break;
				}
			}
			//连接的客户端已经超出最大值
			if(i==FD_SETSIZE){
				close(listenfd);
				handle_err("too many connfd\n");
			}
			//添加select监听新的文件fd
			FD_SET(tmpfd,&fdset);
			//更新最大的fd
			if(tmpfd>maxfd)
				maxfd = tmpfd;
			//根据connfd数组的最大下标
			if(i>maxIndex)
				maxIndex = i;
			//判断是否还有就绪的fd
			if(--readyfdNum==0)
				continue;
		}

		//遍历connfd，找出就绪的fd
		for(i=0;i<=maxIndex;++i){
			if((tmpfd=connfd[i])<0){
				continue;
			}	
			if(FD_ISSET(tmpfd,&tmpset)){
				int ret = Read(tmpfd,buf,sizeof(buf));	
				if(ret<=0){
					Close(tmpfd);
					FD_CLR(tmpfd,&fdset);
					connfd[i] = -1;
				}else{
					int j;
					for(j = 0;j<ret;++j){
						buf[j] = toupper(buf[j]);
					}
					Write(tmpfd,buf,ret);
					memset(buf,0,sizeof(buf));
				}
				if(--readyfdNum==0)
					break;
			}
		}	
	}
	close(listenfd);
	return 0;
}
