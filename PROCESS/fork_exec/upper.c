#include <stdio.h>

int main(){
	char ch;
	while((ch = getchar())!=EOF){
		putchar(toupper(ch));
	}
	return 0;
}
