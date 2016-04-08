#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc,char **argv){
	if(argc<2){
		fprintf(stderr,"%s <pathname>\n",argv[0]);
		exit(-1);
	}
	struct stat buf;
	int ret = stat(argv[1],&buf);
	if(ret==-1){
		perror("stat error");
		exit(-1);
	}
	printf("stat size = %d\n",buf.st_size);
	return 0;
	
}
