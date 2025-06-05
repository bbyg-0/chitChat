#pragma once

#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

typedef struct param{
	int serverSocket;
	int clientSocket;
	socklen_t addrlen;
	struct sockaddr_in address;
} paramThread;

void inisialisasiParamThread(paramThread * param);

void isiPort(paramThread * param, int port);

void * preparingServerSocket(void * vParamT);

void * gettingClient(void * vParamT);


void * sendMessage (void * client);

void * getMessage (void * client);
