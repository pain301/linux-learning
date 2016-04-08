#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc,char **argv){
	if(argc<2){
		fprintf(stderr,"%s <filename> \n",argv[0]);
		exit(0);
	}	
	if(access(argv[1],F_OK)<0){
		perror("F_OK");
		exit(-1);
	}
	printf("file exist\n");
	if(access(argv[1],R_OK)<0){
		perror("R_OK");
		exit(-2);
	}
	printf("file can read\n");
	if(access(argv[1],W_OK)<0){
		perror("W_OK");
		exit(-3);
	}
	printf("file can write\n");
	if(access(argv[1],X_OK)<0){
		perror("X_OK");
		exit(-2);
	}
	printf("file can exec\n");
	return 0;
}
