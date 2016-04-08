#include <stdio.h>
#include <stdlib.h>

#define MODE 0644

int main(int argc,char **argv){
	if(argc<2){
		fprintf(stderr,"%s fifoname\n",argv[0]);
		exit(-1);
	}
	int ret = mkfifo(argv[1],MODE);
	if(ret<0){
		perror("mkfifo failue");
		exit(-2);
	}
	return 0;
}
