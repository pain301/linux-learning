#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc,char **argv){
	if(argc<2){
		fprintf(stderr,"%s fifoname\n",argv[0]);
		exit(-1);
	}
	int ret = unlink(argv[1]);
	if(ret<0){
		perror("unlink error");
		exit(-1);
	}
	return 0;
}
