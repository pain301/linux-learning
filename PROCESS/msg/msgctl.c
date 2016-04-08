#include <sys/types.h>
#include <sys/msg.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <strings.h>

int main(){
	int msgid;
	msgid = msgget(0x1234,0666);
	if(msgid==-1){
		if(errno==ENOENT){
			printf("消息队列不存在\n");
		}else{
			perror("msgget error");
		}
		return 0;	
	}
	printf("msgid:%d\n",msgid);

	int ret;
	struct msqid_ds buf;
	bzero(&buf,sizeof(struct msqid_ds));
	//获取消息队列信息
	ret = msgctl(msgid,IPC_STAT,&buf);
	if(ret==-1){
		perror("msgctl error");
		return 0;
	}
	printf("权限位：%o\n",buf.msg_perm.mode);
	printf("当前消息队列中的字节数：%ld\n",buf.__msg_cbytes);
	printf("当前消息队列中消息的个数：%d\n",buf.msg_qnum);

	#if 0
	//修改消息队列的权限位
	buf.msg_perm.mode = 0644;
	ret = msgctl(msgid,IPC_SET,&buf);
	if(ret==-1){
		perror("msgctl error");
		return 0;
	}

	//删除消息队列
	ret = msgctl(msgid,IPC_RMID,NULL);
	if(ret==-1){
		perror("msgctl error");
		return 0;
	}else{
		printf("delete success!\n");
	}
	#endif
	return 0;
}
