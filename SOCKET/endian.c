#include <stdio.h>

int main(){
	unsigned int num = 0x12345678;
	char *p = (char *)&num;
	int i = 0;
	for(;i<4;i++){
		printf("%x ",p[i]);
	}
	puts("");
	num = htonl(num);
	p = (char *)&num;
	for(i=0;i<4;++i){
		printf("%x ",p[i]);
	}
	puts("");
	return 0;
}
