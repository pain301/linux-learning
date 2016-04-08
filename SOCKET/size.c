#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>

int main(){
	printf("sizeof sockaddr = %d\n",sizeof(struct sockaddr));
	printf("sizeof sockaddr_in = %d\n",sizeof(struct sockaddr_in));
	printf("sizeof in_addr = %d\n",sizeof(struct in_addr));
	return 0;
}
