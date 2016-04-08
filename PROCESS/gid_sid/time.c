#include <stdio.h>
#include <time.h>

int main(){
	char buf[1024] = {0};
	time_t t;
	//获取距离1970.1.1的seconds
	time(&t);
	//将时间转换为当前时间
	ctime_r(&t,buf);
	printf("time: %s",buf);
	return 0;
}
