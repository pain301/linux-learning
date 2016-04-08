#include <sys/shm.h>
#include <errno.h>
#include <stdio.h>

struct Student{
	int age;
	char name[20];
};

int main(){
	int shmid;
	//不存在则会报错
	//shmid = shmget(0x1234,sizeof(struct Student),0666);
	//若共享内存存在则使用，若不存在则创建
	shmid = shmget(0x1234,sizeof(struct Student),0666|IPC_CREAT);
	//shmid = shmget(0x1234,sizeof(struct Student),0666|IPC_CREAT|IPC_EXCL);
	if(shmid==-1){
		if(errno==ENOENT){
			printf("共享内存不存在\n");
		}else if(errno==EEXIST){
			printf("共享内存已经存在\n");
		}else{
			perror("sheget error\n");
		}
		return -1;
	}
	
	return 0;
}
