#include <signal.h>
#include <unistd.h>
#include <stdio.h>

void print_sigset(const sigset_t *);

int main(){
	sigset_t oldset;
	sigset_t newset;
	//屏蔽SIGINT信号
	sigemptyset(&newset);
	sigaddset(&newset,SIGINT); 
	sigprocmask(SIG_BLOCK,&newset,NULL);
	int i = 0;
	while(1){
		//获取当前的信号屏蔽字
		sigpending(&oldset);
		print_sigset(&oldset);
		if(i==5){
			printf("sigset\n");
			//取消屏蔽SIGINT字
			print_sigset(&newset);
			sigprocmask(SIG_UNBLOCK,&newset,NULL);
		}
		i++;
		sleep(1);
	}
	return 0;
}

/*打印信号屏蔽字*/
void print_sigset(const sigset_t *set){
	int i;
	for(i = 1;i<32;i++){
		if(sigismember(set,i)){
			putchar('1');
		}else{
			putchar('0');
		}
	}
	puts("");
}
