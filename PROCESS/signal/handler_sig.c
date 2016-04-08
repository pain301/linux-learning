#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

void print_sigset(const sigset_t *);

void do_sig(int num){
	//获取当前的信号屏蔽字
	sigset_t sig;
	sigprocmask(SIG_SETMASK,NULL,&sig);
	print_sigset(&sig);
	printf("sig num = %d\n",num);
	int i = 5;
	while(i--){
		printf("do_sig function\n");
		sleep(1);
	}
}

void print_sigset(const sigset_t *set){
	int i = 1;
	for(;i<32;i++){
		if(sigismember(set,i))
			putchar('1');
		else
			putchar('0');
	}
	puts("");
}

int main(){
	struct sigaction act;

	//设置函数捕捉动作
	act.sa_handler = do_sig;
	//设置为默认动作
	//act.sa_handler = SIG_DFL;
	//设置为忽略动作
	//act.sa_handler = SIG_IGN;

	//清空临时阻塞信号集，运行捕捉函数的时候屏蔽SIGQUIT
	sigemptyset(&act.sa_mask);
	sigaddset(&act.sa_mask,SIGQUIT);
	print_sigset(&act.sa_mask);
	//选用sa_handler函数指针
	act.sa_flags = 0;
	//设置信号捕捉
	if(sigaction(SIGINT,&act,NULL)<0){
		perror("sigaction error");
		exit(-1);
	}
	while(1){
		printf("*******************\n");
		sleep(1);
	}
	return 0;
}
