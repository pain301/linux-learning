#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc,char **argv){
	if(argc<2){
		fprintf(stderr,"%s <filename> \n",argv[0]);
		exit(-1);
	}
	struct stat buf;
	if(lstat(argv[1],&buf)<0){
		perror("stat error");
		exit(-2);
	}
	printf("%s size = %d\n",argv[1],buf.st_size);
	return 0;
}
