#include <sys/types.h>
#include <sys/msg.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>

struct msgbuf{
	long mtype;
	char mtext[1024];
};

int main(int argc,char **argv){
	int msgid;
	int ret = 0;
	
	if(argc<2){
		fprintf(stderr,"%s msgtype\n",argv[0]);
		return 0;
	}

	msgid = msgget(0x1234,0666);
	if(msgid==-1){
		if(errno==ENOENT){
			printf("消息队列不存在\n");
		}else{
			perror("msgget error");
		}
		return -1;
	}
	
	int type = atoi(argv[1]);
	struct msgbuf buf;
	ret = msgrcv(msgid,&buf,1024,type,IPC_NOWAIT);
	if(ret<0){
		if(errno==EAGAIN){
			printf("no message in queue\n");
		}else{
			perror("msgsnd error");
		}
		return 0;
	}
	buf.mtext[ret] = '\0';
	printf("%s\n",buf.mtext);
	return 0;
}
