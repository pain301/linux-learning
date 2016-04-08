#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main(){
	char ip[] = "192.168.0.1";
	
	struct in_addr addr;
	
	//IPV4
	int ret = inet_aton(ip,&addr);
	printf("%x\n",addr.s_addr);
	addr.s_addr = 0x100a8c0;
	char *p = inet_ntoa(addr);
	printf("%s\n",p);
	
	printf("%x\n",inet_addr(ip));
	
	printf("\n");
	
	//IPV4与IPV6
	ret = inet_pton(AF_INET,ip,&addr);
	printf("%x\n",addr.s_addr);
	inet_ntop(AF_INET,&addr,ip,16);
	printf("%s\n",ip);
	return 0;
}
