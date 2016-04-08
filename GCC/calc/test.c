#include <stdio.h>
#include <stdlib.h>
#include "calc.h"

int main(int argc,char **argv){
	if(argc<3){
		fprintf(stderr,"%s num1,num2\n",argv[0]);
		exit(-1);
	}
	int a = atoi(argv[1]);
	int b = atoi(argv[2]);
	printf("sum: %d\n",add(a,b));
	printf("sub: %d\n",sub(a,b));
	printf("mul: %d\n",mul(a,b));
	printf("dive: %d\n",dive(a,b));
	printf("hello world\n");
	return 0;
}
