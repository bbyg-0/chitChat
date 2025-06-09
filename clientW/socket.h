#pragma once
#include <winsock2.h>
#include <stdio.h>

#pragma comment(lib, "ws2_32.lib") // Link Winsock Library

typedef struct parameterThread{
	SOCKET serverSocket;
	SOCKET clientSocket;
	int addrlen;
	struct sockaddr_in address;
	char * litAddress;
	char socketStatus;
} paramThread;

void inisialisasiParamThread(paramThread * param);

void isiPort(paramThread * param, int PORT);

void inisialisasiSocket(WSADATA * wsa);

DWORD WINAPI clientSocket(LPVOID param);

DWORD WINAPI sendMessage(LPVOID param);

DWORD WINAPI getMessage(LPVOID param);
