#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

void print_err(const char *err,int exitno){
	perror(err);
	exit(exitno);
}

int main(int argc,char **argv){
	if(argc<2){
		fprintf(stderr,"%s filename\n",argv[0]);
		exit(-1);
	}
	int fd;
	fd = open(argv[1],O_RDWR);
	if(fd<0){
		print_err("open error",-2);
	}
	struct flock f_lock;
	f_lock.l_type = F_WRLCK;
	f_lock.l_whence = SEEK_SET;
	f_lock.l_start = 0;
	//对整个文件加锁
	f_lock.l_len = 0;
	
	fcntl(fd,F_SETLKW,&f_lock);
	printf("lock file\n");
	sleep(10);
	f_lock.l_type = F_UNLCK;
	fcntl(fd,F_SETLKW,&f_lock);
	printf("unlock file\n");
	close(fd);
	return 0;
}
