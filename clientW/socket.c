#include <string.h>
#include "socket.h"

void inisialisasiSocket(WSADATA * wsa){
	while(1){
		if (WSAStartup(MAKEWORD(2,2), wsa) != 0) Sleep(500);
		else break;
	}
}

void inisialisasiParam(paramThread * param){
	if((param) == NULL) return;

	(param)->serverSocket = INVALID_SOCKET;
	(param)->clientSocket = INVALID_SOCKET;
	(param)->addrlen = -1;
	(param)->litAddress = NULL;
	(param)->socketStatus = '\0';
}

void isiPort(paramThread * param, int PORT){
	if((param) == NULL) return;
	(param)->address.sin_port = htons(PORT);
}

DWORD WINAPI clientSocket(LPVOID paraM){
	SOCKET sock;

	paramThread * param = (paramThread *)paraM;

	char message[1024], server_reply[1024];
	int recv_size;

	while(1){
		// Buat socket
		while(1){

		(param)->clientSocket = socket(AF_INET, SOCK_STREAM, 0);
			if ((param)->clientSocket == INVALID_SOCKET) Sleep(500);
			else break;
		}
	
		// Siapkan alamat server
		(param)->address.sin_addr.s_addr = inet_addr("192.168.0.104");
		(param)->address.sin_family = AF_INET;
	
		// Connect ke server
		while(1){
		printf("asdasd\n");
			if (connect((param)->clientSocket,
					(struct sockaddr *)&(param)->address,
					sizeof((param)->address)) < 0) Sleep(500);
			else break;
		}
	
		printf("Connected to server.\n");

		(param)->socketStatus = 'c';

		while((param)->socketStatus == 'c') Sleep(5000);
	
		closesocket((param)->clientSocket);
	}
	return 0;
}

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

DWORD WINAPI getMessage(LPVOID paramT){
	char server_reply[100] = {0};

	paramThread * param = (paramThread *)paramT;

	int recv_size = 0;

	while(1){
		while((param)->socketStatus == 'c'){
		recv_size = recv((param)->clientSocket, server_reply, sizeof(server_reply), 0);
		if (recv_size == 0) {
			(param)->socketStatus == 'x';
			break;
		} else {
			server_reply[recv_size] = '\0';
			printf("Server reply: %s\n", server_reply);
		}
		}
		Sleep(1000);
	}
	return 0;
}
