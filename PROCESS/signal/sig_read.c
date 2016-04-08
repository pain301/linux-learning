#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>

void handle_sig(int signo){
	printf("sigaction signo = %d\n",signo);
	int i = 0;
	while(i<6){
		printf("while %d times\n",i++);
	}
}

void toUpper(char * str){
	while(*str){
		//toupper只是传入，不传出
		*str = toupper(*str);
		str++;
	}
}

int main(){
	struct sigaction act;
	//设置SA_RESTART标识，不因信号处理而返回读取失败
	act.sa_flags = 0|SA_RESTART;
	act.sa_handler = handle_sig;
	sigemptyset(&act.sa_mask);
	sigaddset(&act.sa_mask,SIGQUIT);
	sigaction(SIGINT,&act,NULL);
	char buf[1024] = {0};
	do{
		//换行符也读取了
		int ret = read(STDIN_FILENO,buf,sizeof(buf));
		if(ret<0){
			perror("read failed");
			exit(-1);
		}
		toUpper(buf);
		write(STDOUT_FILENO,buf,ret);
	}while(strcmp(buf,"quit\n")!=0);
	return 0;
}
