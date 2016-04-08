#include <sys/msg.h>
#include <sys/types.h>
#include <errno.h>
#include <stdio.h>

int main(){
	int msgid;
	//打开消息队列，若不存在则报错
	//msgid = msgget(0x1234,0666);
	//消息存在则打开消息，不存在，就创建消息
	//msgid = msgget(0x1234,0666|IPC_CREAT);
	//消息存在则提示已存在，消息不存在则创建，单独使用IPC_EXCL无意义
	msgid = msgget(0x1234,0666|IPC_CREAT|IPC_EXCL);
	//每次创建的消息队列都不一样，只能在有血缘关系的进程间使用
	//msgid = msgget(IPC_PRIVATE,0666);
	if(msgid==-1){
		if(errno==ENOENT){
			printf("消息队列不存在\n");
		}else if(errno==EEXIST){
			printf("消息队列已经存在\n");
		}else{
			perror("msgget error");
		}
		return 0;
	}
	printf("消息创建成功 msgid = %d\n",msgid);
	return 0;
}
