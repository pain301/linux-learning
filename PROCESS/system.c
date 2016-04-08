#include <stdio.h>
#include <stdlib.h>

int main(int argc,char **argv){
	if(argc==1){
		fprintf(stderr,"%s command\n",argv[0]);
		exit(1);
	}
	system(argv[1]);
	printf("%s print\n",argv[0]);
	return 0;
}
