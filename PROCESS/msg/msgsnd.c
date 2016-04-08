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

	if(argc<3){
		fprintf(stderr,"%s msgtype msgdata\n",argv[0]);
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
	int len = atoi(argv[2]);
	struct msgbuf buf;
	bzero(&buf,sizeof(struct msgbuf));
	if(type==1){
		buf.mtype = type;
		strcpy(buf.mtext,"111111111111111111111111");
	}else if(type == 2){
		buf.mtype = type;
		strcpy(buf.mtext,"222222222222222222222222");
	}else if(type==3){
		buf.mtype = type;
		strcpy(buf.mtext,"333333333333333333333333");
	}
	ret = msgsnd(msgid,&buf,len,IPC_NOWAIT);
	if(ret<0){
		perror("msgsnd error");
		return 0;
	}
	return 0;
}
