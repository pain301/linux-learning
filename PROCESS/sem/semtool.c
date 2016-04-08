#include <sys/types.h>
#include <sys/sem.h>
#include <stdio.h>
#include <errno.h>

union semun {
	int  val; 
	struct semid_ds *buf;   
	unsigned short  *array;
	struct seminfo  *__buf;
};

int sem_create(key_t key){
	int semid = 0;
	semid = semget(key,1,0666|IPC_CREAT|IPC_EXCL);
	if(semid==-1){
		if(errno==ENOENT){
			printf("信号量不存在\n");
		}else if(errno==EEXIST){
			printf("信号量已经存在\n");
		}else{
			perror("semget error");
		}
	}
	return semid;
}

int sem_setval(int semid,int val){
	union semun su;
	su.val = val;
	int ret = semctl(semid,0,SETVAL,su);
	if(ret==-1){
		perror("semctl error");
	}
	return ret;
}

int sem_getval(int semid){
	union semun su;
	int ret = semctl(semid,0,GETVAL,su);
	if(ret==-1){
		perror("semctl error");
	}
	printf("val = %d\n",su.val);
	return ret;
}

int sem_destroy(int semid){
	int ret = semctl(semid,0,IPC_RMID,0);
	if(ret==-1){
		perror("semctl error");
	}
	return ret;
}

int sem_p(int semid){
	struct sembuf s = {0,-1,0};
	int ret = semop(semid,&s,1);
	if(ret==-1){
		perror("semop error");
	}
	return ret;
}

int sem_v(int semid){
	struct sembuf s = {0,1,0};
	int ret = semop(semid,&s,1);
	if(ret==-1){
		perror("semop error");
	}
	return ret;
}

//getopt();

int main(){
	int semid;
	semid = sem_create(0x1234);
	sem_setval(semid,2);		
	sem_getval(semid);
	sleep(3);
	sem_destroy(semid);
	return 0;
}
