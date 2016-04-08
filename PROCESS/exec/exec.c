#include <stdio.h>
#include <unistd.h>

int main(){
	//execl("./add","add","3","4",NULL);
	
	//需要命令路径
	//execl("/bin/ls","ls","-l",NULL);
	//不需要命令路径，去PATH变量中寻找
	//execlp("ls","ls","-l",NULL);
	
	char *argv[] = {"ls","-l",NULL};
	//参数存放在数组中
	//execv("/bin/ls",argv);
	execvp("ls",argv);
	printf("hello world\n");
	return 0;
}
