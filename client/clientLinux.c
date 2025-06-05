#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

#include "../common.h"
#include "client.h"

#define PORT 8080

int main(int argc, char const* argv[]){
	int opt = 1;
	int status, valread, client_fd;
	struct sockaddr_in serv_addr;
	char buffer[1024] = { 0 };

	pthread_t prepClient, getServer, send, get;



	paramThread param;
	inisialisasiParamThread(&param);
	isiPort(&param, PORT);
	isiAddress(&param, "127.0.0.1");

	pthread_create(&prepClient, NULL, preparingClientSocket, (void *)&param);
	pthread_join(prepClient, NULL);

	pthread_create(&getServer, NULL, gettingServer, (void *)&param);
	pthread_join(getServer, NULL);

	pthread_create(&send, NULL, sendMessage, (void *)&(param.clientSocket));
	pthread_create(&get, NULL, getMessage, (void *)&(param.clientSocket));

	getchar();

	pthread_join(send, NULL);
	pthread_join(get, NULL);

	// closing the connected socket
	close(client_fd);
	return 0;
}
