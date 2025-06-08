// File: client.c
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <stdio.h>

#pragma comment(lib, "ws2_32.lib") // Link Winsock Library

typedef struct param{
	SOCKET serverSocket;
	SOCKET clientSocket;
	int addrlen;
	struct sockaddr_in address;
	char * litAddress;
	char socketStatus;
} paramThread;

void clientSocket(paramThread * param){
	WSADATA wsa;
	SOCKET sock;

	paramThread param;

	char message[1024], server_reply[1024];
	int recv_size;

	// Inisialisasi Winsock
	printf("Initializing Winsock...\n");
	if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
		printf("Failed. Error Code: %d\n", WSAGetLastError());
		return 1;
	}

	// Buat socket
	(param)->clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if ((param)->clientSocket == INVALID_SOCKET) {
		printf("Could not create socket. Error: %d\n", WSAGetLastError());
		return 1;
	}

	// Siapkan alamat server
	(param)->address.sin_addr.s_addr = inet_addr("192.168.0.104");
	(param)->address.sin_family = AF_INET;
	(param)->address.sin_port = htons(8080);

	// Connect ke server
	if (connect((param)->clientSocket, (struct sockaddr *)&(param)->address, sizeof((param)->address)) < 0) {
		printf("Connect failed. Error: %d\n", WSAGetLastError());
		return 1;
	}

	printf("Connected to server.\n");

	// Kirim pesan
	strcpy(message, "Hello from client!");
	send((param)->clientSocket, message, strlen(message), 0);

	// Terima balasan
	recv_size = recv((param)->clientSocket, server_reply, sizeof(server_reply), 0);
	if (recv_size == SOCKET_ERROR) {
		printf("Recv failed. Error: %d\n", WSAGetLastError());
	} else {
		server_reply[recv_size] = '\0';
		printf("Server reply: %s\n", server_reply);
	}

	closesocket((param)->clientSocket);
	WSACleanup();

}

int main() {
	paramThread param;

	clientSocket(&param);

	return 0;
}

