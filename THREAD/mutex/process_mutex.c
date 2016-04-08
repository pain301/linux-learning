#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

struct pmutex{
	int num;
	pthread_mutex_t mutex;
	pthread_mutexattr_t mutexattr;
};

int main(){
	int fd;
	struct pmutex *pm;
	pid_t pid;
	int err;
	
	fd = open("test",O_CREAT|O_RDWR,0777);
	//截断文件大小为pmutex结构体大小
	ftruncate(fd,sizeof(struct pmutex));
	//文件的内存映射，共享模式
	pm = mmap(NULL,sizeof(struct pmutex),PROT_READ|PROT_WRITE,
			MAP_SHARED,fd,0);
	close(fd);
	memset(pm,0,sizeof(struct pmutex));
	//初始化互斥锁属性
	pthread_mutexattr_init(&pm->mutexattr);
	//设置互斥锁属性为进程锁
	pthread_mutexattr_setpshared(&pm->mutexattr,PTHREAD_PROCESS_SHARED);
	//初始化互斥锁
	pthread_mutex_init(&pm->mutex,&pm->mutexattr);
	pid = fork();
	if(pid==0){
		int i = 0;
		for(;i<10;i++){
			pthread_mutex_lock(&pm->mutex);
			(pm->num)++;
			printf("child process num = %d\n",pm->num);
			pthread_mutex_unlock(&pm->mutex);
			sleep(1);
		}
	}else if(pid>0){
		int i = 0;
		for(;i<10;i++){
			pthread_mutex_lock(&pm->mutex);
			(pm->num)+=2;
			printf("parent process num = %d\n",pm->num);
			pthread_mutex_unlock(&pm->mutex);
			sleep(1);
		}
		wait(NULL);
	}
	err = pthread_mutex_destroy(&pm->mutex);
	if(err!=0){
		printf("%s\n",strerror(err));
	}
	//父子进程都需要释放内存映射
	munmap(pm,sizeof(struct pmutex));
	err = unlink("test");
	if(err<0){
		perror("unlink error");
	}
	return 0;
}
