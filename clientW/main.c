// File: client.c

#include "socket.h"

int main() {
	HANDLE thread;
	DWORD threadId;
	paramThread param;

	thread = CreateThread(NULL, 0, clientSocket, (LPVOID)&param, 0, &threadId);

	WaitForSingleObject(thread, INFINITE);
	CloseHandle(thread);

	return 0;
}

