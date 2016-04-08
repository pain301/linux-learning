#include <sys/shm.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

struct Student{
	int age;
	char name[20];
};

int main(){
	int shmid;
	//打开大小必须与存在的内存大小一致
	shmid = shmget(0x1234,sizeof(struct Student),0666|IPC_CREAT);
	if(shmid==-1){
		perror("shmget error");
		return -1;
	}
	struct Student *p = NULL;
	p = shmat(shmid,NULL,0);
	if((int)p==-1){
		perror("shmat error");
		return -1;
	}
	struct Student stu = {25,"pain"};
	memcpy(p,&stu,sizeof(struct Student));
	
	sleep(5);
	//脱离共享内存

	printf("age = %d\n",p->age);
	printf("name = %s\n",p->name);	

	shmdt(p);
	//标记销毁共享内存
	shmctl(shmid,IPC_RMID,NULL);
	return 0;
}
