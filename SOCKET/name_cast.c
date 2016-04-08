#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc,char **argv){
	char *ptr;
	char **pptr;
	struct hostent *hptr;
	char str[32] = {0};
	ptr = argv[1];
	
	//主机名转换为ip地址
	if((hptr = gethostbyname(ptr))==NULL){
		printf("get host by name error %s\n",ptr);	
		exit(-1);
	}
	//正式主机名
	printf("hostname :%s\n",hptr->h_name);
	//打印主机别名
	for(pptr = hptr->h_aliases;*pptr!=NULL;pptr++){
		printf("alias: %s\n",*pptr);
	}
	switch(hptr->h_addrtype){
		case AF_INET:
		case AF_INET6:
			pptr = hptr->h_addr_list;
			for(;*pptr!=NULL;++pptr){
				printf("address :%s\n",inet_ntop(hptr->h_addrtype,*pptr,str,sizeof(str)));
			}
			printf("first address :%s\n",inet_ntop(hptr->h_addrtype,hptr->h_addr,str,sizeof(str)));
		break;
		default:
			printf("unknow type\n");
	}
	return 0;
}
