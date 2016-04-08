#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(){
	int fd[2];
	if(pipe(fd)<0){
		perror("pipe error");
		exit(-1);
	}
	printf("pipe buf size = %ld\n",fpathconf(fd[0],_PC_PIPE_BUF));
	printf("stdout fd num = %ld\n",fpathconf(STDOUT_FILENO,_PC_NAME_MAX));
	printf("size = %ld\n",pathconf("hello",_PC_NAME_MAX));
	return 0;
}
