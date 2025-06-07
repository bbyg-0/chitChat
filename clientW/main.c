// File: client.c
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <stdio.h>

#pragma comment(lib, "ws2_32.lib") // Link Winsock Library

int main() {
	WSADATA wsa;
	SOCKET sock;
	struct sockaddr_in server;
	char message[1024], server_reply[1024];
	int recv_size;

	// Inisialisasi Winsock
	printf("Initializing Winsock...\n");
	if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
		printf("Failed. Error Code: %d\n", WSAGetLastError());
		return 1;
	}

	// Buat socket
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) {
		printf("Could not create socket. Error: %d\n", WSAGetLastError());
		return 1;
	}

	// Siapkan alamat server
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_family = AF_INET;
	server.sin_port = htons(8888);

	// Connect ke server
	if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
		printf("Connect failed. Error: %d\n", WSAGetLastError());
		return 1;
	}

	printf("Connected to server.\n");

	// Kirim pesan
	strcpy(message, "Hello from client!");
	send(sock, message, strlen(message), 0);

	// Terima balasan
	recv_size = recv(sock, server_reply, sizeof(server_reply), 0);
	if (recv_size == SOCKET_ERROR) {
		printf("Recv failed. Error: %d\n", WSAGetLastError());
	} else {
		server_reply[recv_size] = '\0';
		printf("Server reply: %s\n", server_reply);
	}

	closesocket(sock);
	WSACleanup();
	return 0;
}

