#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

#define MAX_PATH 1024

void fsize(const char *);
void dirwalk(const char *,void(*)(const char *));

int main(int argc,char **argv){
	if(argc==1){
		fsize(".");
	}else{
		fsize(argv[1]);
	}
	return 0;
}

void fsize(const char *filename){
	struct stat tmp;
	if(stat(filename,&tmp)==-1){
		fprintf(stderr,"stat: can't access %s\n",filename);
		exit(-1);
	}
	//判断是否为目录
	if((tmp.st_mode&S_IFMT)==S_IFDIR){
		dirwalk(filename,fsize);
	}
	printf("%d %s\n",tmp.st_size,filename);
}

void dirwalk(const char * dirname,void (*fun)(const char *)){
	char name[MAX_PATH];

	//记录项指针
	struct dirent *dp;
	DIR *dir;

	if((dir=opendir(dirname))==NULL){
		fprintf(stderr,"opendir :can't access %s\n",dirname);
		exit(-1);
	}
	//读取记录项
	while((dp=readdir(dir))!=NULL){
		if(strcmp(dp->d_name,".")==0||strcmp(dp->d_name,"..")==0){
			continue;
		}
		//'/'字符与'\0'字符
		if(strlen(dirname)+strlen(dp->d_name)+2>sizeof(name)){
			fprintf(stderr,"dirwalk:name %s %s too long \n",dirname,dp->d_name);
		}else{
			sprintf(name,"%s/%s",dirname,dp->d_name);
			(*fun)("./stat.c");
		}
	}
	closedir(dir);
}
