#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

#include "../common/common.h"
#include "../common/socket.h"

#define PORT 8080
#define ADDRESS "127.0.0.1"

int main(int argc, char const* argv[]){
	pthread_t prepClient, getServer, send, get;

	paramThread param;
	inisialisasiParamThread(&param);
	isiPort(&param, PORT);
	isiAddress(&param, ADDRESS);


	pthread_create(&prepClient, NULL, preparingClientSocket, (void *)&param);
	pthread_join(prepClient, NULL);


	pthread_create(&getServer, NULL, gettingServer, (void *)&param);
	pthread_create(&send, NULL, sendMessage, (void *)&(param.clientSocket));
	pthread_create(&get, NULL, getMessage, (void *)&(param.clientSocket));

	pthread_join(getServer, NULL);
	pthread_join(send, NULL);
	pthread_join(get, NULL);

	// closing the connected socket
	close((param).clientSocket);

	return 0;
}
