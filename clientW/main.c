#include <winsock2.h>
#include <stdio.h>

#define PORT 8080
#define ADDRESS "192.168.116.143"

DWORD WINAPI func (LPVOID vParam){
	while(1){
		printf("HOLAAAA");
		Sleep(1000);
	}
}

int main(void) {
	HANDLE thread;
	DWORD threadId;

	thread = CreateThread(
		NULL,		 // default security attributes
		0,			// default stack size
		func, // function to run
		NULL,		 // argument to thread function
		0,			// default creation flags
		&threadId);   // receive thread identifier

	WaitForSingleObject(thread, INFINITE);
	CloseHandle(thread);

	return 0;
}

