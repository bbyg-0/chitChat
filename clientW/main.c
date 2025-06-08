// File: client.c

#include "socket.h"

int main() {
	HANDLE cliSocket, sendMSG, getMSG;
	DWORD cliSocketId, sendMSGId, getMSGId;
	paramThread param;

	cliSocket = CreateThread(NULL, 0, clientSocket, (LPVOID)&param, 0, &cliSocketId);
	sendMSG = CreateThread(NULL, 0, sendMessage, (LPVOID)&param, 0, &sendMSGId);
	getMSG = CreateThread(NULL, 0, getMessage, (LPVOID)&param, 0, &getMSGId);


	WaitForSingleObject(cliSocket, INFINITE);
	CloseHandle(cliSocket);

	WaitForSingleObject(sendMSG, INFINITE);
	CloseHandle(sendMSG);

	WaitForSingleObject(getMSG, INFINITE);
	CloseHandle(getMSG);

	return 0;
}

