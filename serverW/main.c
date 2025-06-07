// File: server.c
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <stdio.h>

#pragma comment(lib, "ws2_32.lib") // Link Winsock Library

int main() {
	WSADATA wsa;
	SOCKET server_socket, client_socket;
	struct sockaddr_in server, client;
	int client_len = sizeof(client);
	char buffer[1024];

	// Inisialisasi Winsock
	printf("Initializing Winsock...\n");
	if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
		printf("Failed. Error Code: %d\n", WSAGetLastError());
		return 1;
	}

	// Buat socket
	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket == INVALID_SOCKET) {
		printf("Could not create socket. Error: %d\n", WSAGetLastError());
		return 1;
	}

	// Siapkan struktur server
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(8888);

	// Bind
	if (bind(server_socket, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR) {
		printf("Bind failed. Error: %d\n", WSAGetLastError());
		return 1;
	}

	// Listen
	listen(server_socket, 3);
	printf("Waiting for incoming connections...\n");

	// Accept koneksi
	client_socket = accept(server_socket, (struct sockaddr *)&client, &client_len);
	if (client_socket == INVALID_SOCKET) {
		printf("Accept failed. Error: %d\n", WSAGetLastError());
		return 1;
	}

	printf("Client connected.\n");

	// Terima pesan
	int recv_size = recv(client_socket, buffer, sizeof(buffer), 0);
	if (recv_size == SOCKET_ERROR) {
		printf("Recv failed. Error: %d\n", WSAGetLastError());
	} else {
		buffer[recv_size] = '\0';
		printf("Received: %s\n", buffer);
	}

	// Kirim balasan
	const char* reply = "Message received.";
	send(client_socket, reply, strlen(reply), 0);

	closesocket(client_socket);
	closesocket(server_socket);
	WSACleanup();

	return 0;
}

