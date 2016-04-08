#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc,char **argv){
	if(argc<3){
		fprintf(stderr,"%s arg1 arg2\n",argv[0]);
		exit(-1);
	}
	if(kill((pid_t)atoi(argv[2]),atoi(argv[1]))<0){
		perror("kill error");
		exit(-2);
	}
	return 0;
}
