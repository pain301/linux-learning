#include <signal.h>
#include <stdio.h>

int main(){
	alarm(1);
	printf("sigset_t size = %d\n",sizeof(sigset_t));
	pause();
	printf("wait for alarm\n");
	return 0;
}
