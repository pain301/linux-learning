#include <stdio.h>
#include <signal.h>

void handle_sig(int signo){
	printf("signo = %d\n",signo);
}

int main(){
	//C标准库提供的信号处理设置函数
	signal(SIGINT,handle_sig);
	while(1){
		printf("***************\n");
		sleep(1);
	}
	return 0;
}
