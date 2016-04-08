#预处理器标志，指定头文件在include目录下
CPPFLAGS = -Iinclude
#编译时加调试，开启严格检查
CFLAGS = -g -Wall
#编译器选择gcc
CC = gcc

#找到所有以.c结尾的文件
src = $(wildcard *.c)
#将以.c结尾的文件转换为以.o结尾的文件
obj = $(patsubst %.c,%.o,$(src))
target = test

# $@表示目标
# $^表示所有依赖
# $<表示依赖中的第一个
$(target):$(obj)
	$(CC) $^ -o $@

%.o:%.c
	$(CC) -c $< $(CFLAGS) $(CPPFLAGS) -o $@

#彻底清除
distclean:
	rm /usr/bin/test
#安装
install:
	cp test /usr/bin

.PHONY:clean
clean:
	-rm -f *.o
	-rm -f test
