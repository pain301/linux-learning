#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <stdlib.h>
#include <stdio.h>

int main(){
	char filename[10] ;
	int fd;
	int i = 0;
	while(1){
		sprintf(filename,"file%d",i);
		fd = open(filename,O_WRONLY|O_CREAT,0644);
		if(fd<0){
			perror("open too much file");
			exit(-1);
		}
		i++;
		printf("fd = %d\n",fd);
		//close(fd);
	}
	return 0;
}
