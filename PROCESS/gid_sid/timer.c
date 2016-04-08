#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>

void handle_sig(int signo){
	time_t t;
	time(&t);
	struct tm *p = gmtime(&t);
	printf("%04d-%02d-%02d  %02d:%02d:%02d\n", 
		1900+p->tm_year, 
		1+p->tm_mon, 
		p->tm_mday, 
		8+p->tm_hour, 
		p->tm_min, 
		p->tm_sec);
}

void init_timer(int secs,int usecs){
	struct itimerval timer;
	//开始的时间
	timer.it_value.tv_sec = secs;
	timer.it_value.tv_usec = usecs;
	//间隔时间
	timer.it_interval.tv_sec = secs;
	timer.it_interval.tv_usec = usecs;
	//设置真实计时器
	if(setitimer(ITIMER_REAL,&timer,NULL)<0){
		perror("setitimer failed");
		exit(-1);
	}
}

int main(){
	struct sigaction act;
	act.sa_flags = 0;
	act.sa_handler = handle_sig;
	sigemptyset(&act.sa_mask);
	sigaddset(&act.sa_mask,SIGQUIT);
	sigaction(SIGALRM,&act,NULL);
	//清除之前的闹钟计时
	alarm(0);
	init_timer(1,0);
	while(1){

	}
	return 0;
}
