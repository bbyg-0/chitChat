#include "client.h"
#include "../common.h"

#include <string.h>
#include <errno.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>

void inisialisasiParamThread(paramThread * param){
	if(isEmpty(param)) return;
	(param)->serverSocket = -1;
	(param)->clientSocket = -1;
	(param)->addrlen = 0;
	(param)->litAddress = NULL;
}

void isiPort(paramThread * param, int port){
	if(isEmpty(param)) return;

	(param)->address.sin_port = htons(port);
}

void isiAddress(paramThread * param, char * address){
	if(isEmpty(param)) return;

	(param)->litAddress = myStrdup(address);
}

void * preparingServerSocket(void * vParam){
	int pass = -1;
	int opt = 1;

	paramThread * param = (paramThread *)vParam;

	while(pass < 0){
		usleep(500000);
		((param)->serverSocket) = socket(AF_INET, SOCK_STREAM, 0);
		pass = ((param)->serverSocket);
	}
	pass = -1;
	// Forcefully attaching socket to the port 8080
	while(pass < 0){
		usleep(500000);
		pass = setsockopt(((param)->serverSocket), SOL_SOCKET,
				SO_REUSEADDR | SO_REUSEPORT, 
				&opt, sizeof(opt));
	}
	pass = -1;

	(param)->address.sin_family = AF_INET;
	(param)->address.sin_addr.s_addr = INADDR_ANY;
	
	int flags = fcntl(((param)->serverSocket), F_GETFL, 0);         // Get current flags
								
	while(pass < 0){
		usleep(500000);
		pass = fcntl(((param)->serverSocket), F_SETFL, flags | O_NONBLOCK);
	}
	pass = -1;

	// Forcefully attaching socket to the port 8080
	while(pass < 0){
		usleep(500000);
		pass = bind(((param)->serverSocket), (struct sockaddr*)&(param->address), sizeof((param)->address));
	}
	pass = -1;

	return NULL;
}

void * preparingClientSocket(void * vParam){
	int opt = 1;
	int pass = -1;
	paramThread * param = (paramThread *)vParam;

	while(pass < 0){
		usleep(500000);
		pass = ((param)->clientSocket = socket(AF_INET, SOCK_STREAM, 0));	
	}
	pass = -1;

	while(pass < 0){
		usleep(500000);
		pass = setsockopt(((param)->clientSocket), SOL_SOCKET,
				SO_REUSEADDR | SO_REUSEPORT, 
				&opt, sizeof(opt));
	}
	pass = -1;

	(param)->address.sin_family = AF_INET;
	
	while(pass < 0){
		usleep(500000);
		pass = inet_pton(AF_INET, (param)->litAddress, &(param)->address.sin_addr);
	}
	pass = -1;

	printf("PREPARING CLIENT BERES\n");

	return NULL;
}

void * gettingClient(void * vParam){
	paramThread * param = (paramThread *)vParam;
	int pass = -1;
	while(pass < 0){
		usleep(500000);
		pass = listen(((param)->serverSocket), 3);
	}

	while(1){
		if ((param->clientSocket = accept(((param)->serverSocket), (struct sockaddr*)&(param)->address, &(param)->addrlen)) < 0) {
			if(errno == EAGAIN || errno == EWOULDBLOCK){
				usleep(100000);
				continue;
			}		}else{
			break;
		}
	}

	return NULL;
}

void * gettingServer(void * vParam){
	paramThread * param = (paramThread *)vParam;
	int pass = -1;

	while (pass < 0){
		usleep(50000);
		pass = connect((param)->clientSocket, (struct sockaddr*)&(param)->address, sizeof((param)->address));
		printf("GETTING SERVER\n");
	}
	printf("GETTING SERVER BERES\n");

	return NULL;
}

void * sendMessage (void * c){
	int * client = (int *)(c);
	char buffer[1024] = {0};

	while(1){
		printf("SEND:\t"); secureInputString(buffer, 1024);
		send(*client, buffer, strlen(buffer), 0);
		memset(buffer, '\0', sizeof(buffer));
	}
	return NULL;
}

void * getMessage (void * c){
	int * client = (int *)(c);
	char buffer[1024] = {0};

	while(1){
		if(read(*client, buffer, 1024 - 1) >= 0){
			printf("\nMESSAGE:\t%s\n", buffer);
			memset(buffer, '\0', sizeof(buffer));
		}
	}
	return NULL;
}
