#include "socket.h"

DWORD WINAPI clientSocket(LPVOID paraM){
	WSADATA wsa;
	SOCKET sock;

	paramThread * param = (paramThread *)paraM;

	char message[1024], server_reply[1024];
	int recv_size;

	while(1){
		while(1){
			if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) Sleep(500);
			else break;
		}
	
		// Buat socket
		while(1){
		(param)->clientSocket = socket(AF_INET, SOCK_STREAM, 0);
			if ((param)->clientSocket == INVALID_SOCKET) Sleep(500);
			else break;
		}
	
		// Siapkan alamat server
		(param)->address.sin_addr.s_addr = inet_addr("192.168.0.104");
		(param)->address.sin_family = AF_INET;
		(param)->address.sin_port = htons(8080);
	
		// Connect ke server
		while(1){
			if (connect((param)->clientSocket,
					(struct sockaddr *)&(param)->address,
					sizeof((param)->address)) < 0) Sleep(500);
			else break;
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
	}
	WSACleanup();
	return 0;
}
