#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>

#include "../common/common.h"
#include "../common/socket.h"

#define PORT 8080
int main(int argc, char const* argv[]){
	int server_fd, client;	//socket server ama socket buat client yang diterima

	ssize_t valread;

	struct sockaddr_in address;	//alamat ip
	int opt = 1;

	socklen_t addrlen = sizeof(address);	//panjang alamat ip
	char buffer[1024] = { 0 };		//buffer pesan
	
	pthread_t prepServer, getClient, send, get;
	paramThread param;
	param.serverSocket = server_fd;
	param.clientSocket = client;

	inisialisasiParamThread(&param);
	isiPort(&param, PORT);

	/*
	preparingServerSocket((void *)&param);
	printf("XXXXXXXXX\n");


	*/
	pthread_create(&prepServer, NULL, preparingServerSocket, (void *)&param);
	pthread_join(prepServer, NULL);

	pthread_create(&getClient, NULL, gettingClient, (void *)&param);
	pthread_join(getClient, NULL);

  
	pthread_create(&send, NULL, sendMessage, (void *)&(param.clientSocket));
	pthread_create(&get, NULL, getMessage, (void *)&(param.clientSocket));

	pthread_join(send, NULL);
	pthread_join(get, NULL);

	// closing the connected socket
	close(client);
  
	// closing the listening socket
	close(server_fd);
	return 0;
}
