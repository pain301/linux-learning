#include <signal.h>
#include <stdio.h>

void handle_sig(int signo){

}

unsigned int mysleep(unsigned int secs){
	struct sigaction newact;
	struct sigaction oldact;
	
	sigset_t newmask;
	sigset_t oldmask;
	sigset_t suspendmask;
	
	//设置捕捉函数，并保存原有信息处理动作
	sigemptyset(&newact.sa_mask);
	sigaddset(&newact.sa_mask,SIGQUIT);
	newact.sa_handler = handle_sig;
	newact.sa_flags = 0;
	sigaction(SIGALRM,&newact,&oldact);
	
	//阻塞SIGALRM信号，并且保存原有的信号屏蔽字
	sigemptyset(&newmask);
	sigaddset(&newmask,SIGALRM);
	sigprocmask(SIG_BLOCK,&newmask,&oldmask);

	alarm(secs);
	
	//取消屏蔽字SIGALRM
	//即使在此刻发生调度，由于之前屏蔽了SIGALRM信号
	//再次回到这里时，SIGALRM尚处于未决状态，解开屏蔽字之后，信号唤醒
	suspendmask = oldmask;
	sigdelset(&suspendmask,SIGALRM);
	sigsuspend(&suspendmask);

	//为定时足够的秒数
	unsigned int unsleep = alarm(0);
	
	//恢复原有的信号处理动作与信号屏蔽字
	sigaction(SIGALRM,&oldact,NULL);
	sigprocmask(SIG_SETMASK,&oldmask,NULL);

	return unsleep;
}

int main(){
	int i = 0;
	while(1){
		printf("sleep time is %d\n",i++);	
		mysleep(1);
	}
	return 0;
}

