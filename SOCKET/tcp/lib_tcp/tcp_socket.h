#ifndef __TCP_SOCKET__H
#define __TCP_SOCKET__H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <pthread.h>

#ifdef __cplusplus
extern 'C'
{
#endif

#define TCP_OK 0
#define TCP_ERR 1000
#define TCP_ERRPARAM (TCP_ERR+1)
#define TCP_ERRTIMEOUT (TCP_ERR+2)
#define TCP_ERRPEERCLOSED (TCP_ERR+3)
#define TCP_ERRMALLOC (TCP_ERR+4)

struct packet{
	int len;
	char data[1024];
};

void handle_sig(int );

int client_init(void **handle,int conntime,int sendtime,int recvtime,int connNum);
int client_getConn(void *handle,char *ip,int port,int *connfd);
int client_close(int *connfd);
int client_send(void *handle,int connfd,unsigned char *data,int datalen);
int client_recv(void *handle,int connfd,unsigned char *data,int *datalen);
int client_destroy(void *handle);

#ifdef __cplusplus
}
#endif

#endif
