#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>

int main(){
	char buf[100];
	getcwd(buf,sizeof(buf));
	printf("%s\n",buf);
	return 0;
}
