#include "socket.h"
#include "common.h"

#ifdef _WIN32
#else
#include <netinet/in.h>
#include <sys/socket.h>
#include <pthread.h>
#include <string.h>
#include <arpa/inet.h>
#endif

#include <errno.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

void inisialisasiParamThread(paramThread * param){
#ifdef _WIN32
	if((param) == NULL) return;

	(param)->serverSocket = INVALID_SOCKET;
	(param)->clientSocket = INVALID_SOCKET;
	(param)->addrlen = -1;
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

#ifdef _WIN32
DWORD WINAPI serverSocket(LPVOID paramT){
	paramThread * param = (paramThread *)paramT;

	(param)->addrlen = sizeof((param)->address);

	while(1){
		(param)->serverSocket = socket(AF_INET, SOCK_STREAM, 0);
		if ((param)->serverSocket == INVALID_SOCKET) Sleep(500);
		else break;
	}

	(param)->address.sin_family = AF_INET;
	(param)->address.sin_addr.s_addr = INADDR_ANY;

	while(1){
		if (bind((param)->serverSocket,
			(struct sockaddr *)&(param)->address,
			sizeof((param)->address)) == SOCKET_ERROR) Sleep(500);
		else break;
	}

	while(1){
		if(listen((param)->serverSocket, 3) < 0) Sleep(500);
		else break;
	}

	while(1){
		(param)->clientSocket = accept((param)->serverSocket,
						(struct sockaddr *)&client,
						&(param)->addrlen);
		if ((param)->clientSocket == INVALID_SOCKET) Sleep(500);
		
	}
}
#else
void * serverSocket(void * vParam){
	int opt = 1;

	paramThread * param = (paramThread *)vParam;
	
	while(1){	
		while(1){
			((param)->serverSocket) = socket(AF_INET, SOCK_STREAM, 0);
			if ((param)->serverSocket < 0) usleep(500000);
			else break;

		}
		while(1){
			if(setsockopt(((param)->serverSocket), SOL_SOCKET,
					SO_REUSEADDR | SO_REUSEPORT, 
					&opt, sizeof(opt)) < 0) usleep(500000);
			else break;
		}
	
		(param)->address.sin_family = AF_INET;
		(param)->address.sin_addr.s_addr = INADDR_ANY;
		
		int flags = fcntl(((param)->serverSocket), F_GETFL, 0);         // Get current flags
									
		while(1){
			if(fcntl(((param)->serverSocket), F_SETFL, flags | O_NONBLOCK) >= 0) break;
			else usleep(500000);
		}

		while(1){
			if(bind(((param)->serverSocket),
				(struct sockaddr*)&(param->address),
				sizeof((param)->address)) < 0) usleep(500000);
			else break;
		}
	
		while(1){
			if(listen(((param)->serverSocket), 3) < 0) usleep(500000);
			else break;
		}
	
		printf("SEBELUM LOOP\n");
		while(1){
			while(param->clientSocket < 0){
				if ((param->clientSocket = accept(((param)->serverSocket), 
								(struct sockaddr*)&(param)->address, 
								&(param)->addrlen)) < 0)
					usleep(500000);
				else break;
			}
		}
	}

	return NULL;
}
#endif

#ifdef _WIN32
DWORD WINAPI clientSocket(LPVOID paraM){
	paramThread * param = (paramThread *)paraM;

	while(1){
		while(1){
		(param)->clientSocket = socket(AF_INET, SOCK_STREAM, 0);
			if ((param)->clientSocket == INVALID_SOCKET) Sleep(500);
			else break;
		}
	
		(param)->address.sin_family = AF_INET;
	
		while(1){
			if (connect((param)->clientSocket,
					(struct sockaddr *)&(param)->address,
					sizeof((param)->address)) < 0) Sleep(500);
			else break;
		}
	
		printf("CONNECTED TO SERVER\n");

		(param)->socketStatus = 'c';

		while((param)->socketStatus == 'c') {Sleep(5000);}
		printf("SEBELUM WHILE: %c\n", (param)->socketStatus);

	
		closesocket((param)->clientSocket);
	}
	return 0;
}
#else
void * clientSocket(void * vParam){
	int opt = 1;
	paramThread * param = (paramThread *)vParam;

	while(1){
		isiStatus(param, 'C');
		while(1){
			(param)->clientSocket = socket(AF_INET, SOCK_STREAM, 0);
			if((param)->clientSocket < 0) usleep(500000);
			else break;
		}
	
		while(1){
			if(setsockopt(((param)->clientSocket), SOL_SOCKET,
					SO_REUSEADDR | SO_REUSEPORT, 
					&opt, sizeof(opt)) < 0) usleep(500000);
			else break;
		}
	
		(param)->address.sin_family = AF_INET;
		
		while(1){
			if(inet_pton(AF_INET, (param)->litAddress, &(param)->address.sin_addr) < 0 )
				usleep(500000);
			else break;
		}
	
		// JENIS-JENIS STATUS BUAT CLIENT:
		// C : awalan, gak konek ke server
		// c : konek ke server
		// x : coba konek atau pernah konek tapi gak lagi karena server mati
		while(1){
			if ((param)->socketStatus == 'C'){
				if( connect((param)->clientSocket,
						(struct sockaddr*)&(param)->address, 
						sizeof((param)->address)) >= 0)
					(param)->socketStatus = 'c';
				else (param)->socketStatus = 'x';
			}
			while((param)->socketStatus == 'c') usleep(500000);
			if ((param)->socketStatus == 'x'){ close((param)->clientSocket); break;}
		}
		printf("\nKELUAR LOOP\n"); usleep(1000000);
	}
	return NULL;
}
#endif

#ifdef _WIN32
DWORD WINAPI sendMessage(LPVOID paramT){
	char buffer[100] = {0};

	paramThread * param = (paramThread *)paramT;

	while(1){
		while((param)->socketStatus == 'c'){
		scanf("%s", buffer);
		send((param)->clientSocket, buffer, strlen(buffer), 0);
		memset(buffer, 0, strlen(buffer));
		}
		Sleep(1000);
	}
	return 0;
}
#else
void * sendMessage (void * c){
	printf("INSIDE sendMessage\n");
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
DWORD WINAPI getMessage(LPVOID paramT){
	char server_reply[100] = {0};

	paramThread * param = (paramThread *)paramT;

	int recv_size = 0;

	while(1){
		while((param)->socketStatus == 'c' || (param)->socketStatus == 'x'){
			recv_size = recv((param)->clientSocket, server_reply, sizeof(server_reply), 0);
			if (recv_size == 0) {
				if((param)->socketStatus == 'c') (param)->socketStatus = 'x';
				else (param)->socketStatus = 'S';
				break;
			} else {
				server_reply[recv_size] = '\0';
				printf("Server reply: %s\n", server_reply);
				memset(server_reply, '\0', sizeof(server_reply));
			}
		}
		Sleep(1000);
	}
	return 0;
}
#else
void * getMessage (void * vParam){
	printf("INSIDE getMessage\n");
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
