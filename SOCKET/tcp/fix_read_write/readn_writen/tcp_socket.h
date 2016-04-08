#ifndef __TCP_SOCKET__H
#define __TCP_SOCKET__H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>

struct packet{
	int len;
	char buf[1024];
};

#endif
