#include <stdio.h>
#include <stdlib.h>

int main(int argc,char **argv){
	if(argc<3){
		fprintf(stderr,"%s num1 num2\n",argv[0]);
		exit(-1);
	}
	int a = atoi(argv[1]);
	int b = atoi(argv[2]);
	printf("%d + %d = %d\n",a,b,a+b);
	return 0;
}
