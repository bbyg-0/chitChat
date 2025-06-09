// File: server.c
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <stdio.h>

#include "../common/socket.h"

#pragma comment(lib, "ws2_32.lib") // Link Winsock Library
#define PORT 8080
#define ADDRESS "127.0.0.1"

int main() {
	WSADATA wsa;

	paramThread param;

	inisialisasiParamThread(&param);
	isiPort(&param, PORT);
	isiAddress(&param, ADDRESS);
	isiStatus(&param, 's');

#ifdef _WIN32
	HANDLE servSocket, sendMSG, getMSG;
	DWORD servSocketId, sendMSGId, getMSGId;

	WSADATA wsa;
	WSAStartup(MAKEWORD(2,2), &wsa);

	servSocket = CreateThread(NULL, 0, serverSocket, (LPVOID)&param, 0, &servSocketId);
	sendMSG = CreateThread(NULL, 0, sendMessage, (LPVOID)&param, 0, &sendMSGId);
	getMSG = CreateThread(NULL, 0, getMessage, (LPVOID)&param, 0, &getMSGId);


	WaitForSingleObject(servSocket, INFINITE);
	CloseHandle(servSocket);

	WaitForSingleObject(sendMSG, INFINITE);
	CloseHandle(sendMSG);

	WaitForSingleObject(getMSG, INFINITE);
	CloseHandle(getMSG);
#else
#endif

	WSACleanup();
	return 0;
}

