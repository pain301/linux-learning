#ifndef __TCP_SOCKET__H
#define __TCP_SOCKET__H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <pthread.h>

struct packet{
	int len;
	char data[1024];
};

void handle_sig(int );

#endif
