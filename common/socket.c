#include "socket.h"
#include "common.h"

#ifdef _WIN32
#include <winsock2.h>
#else
#include <pthread.h>
#include <errno.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#endif

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void inisialisasiParamThread(paramThread * param){
#ifdef _WIN32
	if(isEmpty(param)) return;
	(param)->serverSocket = INVALID_SOCKET;
	(param)->clientSocket = INVALID_SOCKET;
	(param)->addrlen = 0;
	(param)->litAddress = NULL;
	(param)->socketStatus = '\0';
#else
	if(isEmpty(param)) return;
	(param)->serverSocket = -1;
	(param)->clientSocket = -1;
	(param)->addrlen = 0;
	(param)->litAddress = NULL;
	(param)->socketStatus = '\0';
#endif
}

void isiStatus(paramThread * param, char status){
	if(isEmpty(param)) return;
	if(status != 's' && status != 'c' && status != 'C'){
		printf("KARAKAKTER YANG DITERIMA HANYA:\n\t- s : server\n\t- c : client\n");
		return;
	}else printf("ISI STATUS BERHASIL\n");

	(param)->socketStatus = status;

}

void isiPort(paramThread * param, int port){
	if(isEmpty(param)) return;

	(param)->address.sin_port = htons(port);
}

void isiAddress(paramThread * param, char * address){
	if(isEmpty(param)) return;

	(param)->litAddress = myStrdup(address);
}

void * serverSocket(void * vParam){
	int pass = -1;
	int opt = 1;

	paramThread * param = (paramThread *)vParam;
	
	while(1){	
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
		while(pass < 0){
			usleep(500000);
			pass = bind(((param)->serverSocket), (struct sockaddr*)&(param->address), sizeof((param)->address));
		}
		pass = -1;
	
		while(pass < 0){
			usleep(500000);
			pass = listen(((param)->serverSocket), 3);
		}
	
		printf("SEBELUM LOOP\n");
		while(1){
			while(param->clientSocket < 0){
				if ((param->clientSocket = accept(((param)->serverSocket), 
								(struct sockaddr*)&(param)->address, 
								&(param)->addrlen)) < 0) {
					if(errno == EAGAIN || errno == EWOULDBLOCK){
						continue;
					}
				}else{
					break;
				}
			}
			usleep(100000);
		}
	}

	return NULL;
}

#ifdef _WIN32
DWORD WINAPI clientSocket(LPVOIF vParam){
	WSADATA wsa;
	SOCKET sock;
	struct sockaddr_in server;
	char message[1024], server_reply[1024];
	int recv_size;

	int opt = 1;
	int pass = -1;
	paramThread * param = (paramThread *)vParam;
	
	while(1){
		isiStatus(param, 'C');
		while(pass < 0){
			pass = WSAStartup(MAKEWORD(2,2), &wsa);
			Sleep(1000);
		}
		pass = -1;
	
		while(pass < 0){
			(param)->clientSocket = socket(AF_INET, SOCK_STREAM, 0);
			if ((param)->clientSocket != INVALID_SOCKET) pass = 0;
		}
		pass = -1;
	
		(param)->address.server.sin_family = AF_INET;
	
		// JENIS-JENIS STATUS BUAT CLIENT:
		// C : awalan, gak konek ke server
		// c : konek ke server
		// x : pernah konek ke server, tapi gak lagi karena server mati
		while(pass == 0){
			while((param)->socketStatus == 'C' && pass == 0){
				Sleep(50);
				if (connect((param)->clientSocket,
						(struct sockaddr *)&(param)->address,
						sizeof((param)->address)) >= 0)
					(param)->socketStatus = 'c';
				else{
					if((param)->socketStatus == 'x'){
						pass = -1;
						close((param)->clientSocket);
						break;
					}
				}
				//GETTING SERVER
			}
			if((param)->socketStatus == 'x'){
				pass = -1;
				close((param)->clientSocket);
				break;
			}
		Sleep(500);
		}
	Sleep(500);
	}
	retutn 0;
}
#else
void * clientSocket(void * vParam){
	int opt = 1;
	int pass = -1;
	paramThread * param = (paramThread *)vParam;

	while(1){
		isiStatus(param, 'C');
		while(pass < 0){
			usleep(500000);
			((param)->clientSocket = socket(AF_INET, SOCK_STREAM, 0));	
			pass = (param)->clientSocket;
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
		pass = 0;
	
		// JENIS-JENIS STATUS BUAT CLIENT:
		// C : awalan, gak konek ke server
		// c : konek ke server
		// x : pernah konek ke server, tapi gak lagi karena server mati
		while(pass == 0){
			while ((param)->socketStatus == 'C' && pass == 0){
				usleep(50000);
				if( connect((param)->clientSocket,
						(struct sockaddr*)&(param)->address, 
						sizeof((param)->address)) >= 0)
					(param)->socketStatus = 'c';
				else {
					if ((param)->socketStatus == 'x'){
						pass = -1;
						close((param)->clientSocket);
						break;
					}
				}
				printf("GETTING SERVER\n");
			}
			if ((param)->socketStatus == 'x'){
				pass = -1;
				close((param)->clientSocket);
				break;
			}
		usleep(1000000);
		}
	printf("KELUAR LOOP\n");
	usleep(1000000);
	}


	return NULL;
}
#endif


#ifdef _WIN32
DWORD WINAPI sendMessage (LPVOID c){
	SOCKET * client = (SOCKET *)(c);
	char buffer[1024] = {0};

	while(1){
		if((*client) < 0){Sleep(50); continue;}
		printf("SEND:\t"); secureInputString(buffer, 1024);
		send(*client, buffer, strlen(buffer), 0);
		memset(buffer, '\0', sizeof(buffer));
	}
	return 0;
}
#else
void * sendMessage (void * c){
	int * client = (int *)(c);
	char buffer[1024] = {0};

	while(1){
		if((*client) < 0){usleep(5000); continue;}
		printf("SEND:\t"); secureInputString(buffer, 1024);
		send(*client, buffer, strlen(buffer), 0);
		memset(buffer, '\0', sizeof(buffer));
	}
	return NULL;
}
#endif

#ifdef _WIN32
DWORD WINAPI getMessage (LPVOID vParam){
	char buffer[1024] = {0};
	int pass = 0;

	paramThread * param = (paramThread *)vParam;

	while(1){
		if((param)->socketStatus == 's' || (param)->socketStatus == 'c'){
		int recv_size = recv(sock, buffer, sizeof(buffer), 0);
		if (recv_size == SOCKET_ERROR) {
			if((param)->socketStatus == 's'){
				close((param)->clientSocket);
				(param)->clientSocket = -1;
			}else if((param)->socketStatus == 'c' || (param)->socketStatus == 'C'){
				(param)->socketStatus = 'x';
			}else return NULL;
		} else {
			printf("\nMESSAGE:\t%s\n", buffer);
			memset(buffer, '\0', sizeof(buffer));
		}
		Sleep(1000);
		}
	}
	return 0;
}
#else
void * getMessage (void * vParam){
	char buffer[1024] = {0};
	int pass = 0;

	paramThread * param = (paramThread *)vParam;

	while(1){
		if((param)->socketStatus == 's' || (param)->socketStatus == 'c'){
		pass = read((param)->clientSocket, buffer, 1024 - 1);

		if(pass > 0){
			printf("\nMESSAGE:\t%s\n", buffer);
			memset(buffer, '\0', sizeof(buffer));
		}else if(pass == 0){
			if((param)->socketStatus == 's'){
				close((param)->clientSocket);
				(param)->clientSocket = -1;
			}else if((param)->socketStatus == 'c' || (param)->socketStatus == 'C'){
				(param)->socketStatus = 'x';
			}else return NULL;
		}
		}
	}
	return NULL;
}
#endif
