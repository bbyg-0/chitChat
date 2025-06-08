#pragma once

#ifdef _WIN32
	#include <winsock2.h>
	#include <windows.h>

	typedef SOCKET socket_t;
	typedef int socklen_custom_t;
#else
	#include <errno.h>
	#include <fcntl.h>
	#include <netinet/in.h>
	#include <sys/socket.h>
	#include <unistd.h>

	typedef int socket_t;
	typedef socklen_t socklen_custom_t;
#endif

typedef struct param {
    socket_t serverSocket;
    socket_t clientSocket;
    socklen_custom_t addrlen;
    struct sockaddr_in address;
    char *litAddress;
    char socketStatus;
} paramThread;


void inisialisasiParamThread(paramThread * param);

void isiStatus(paramThread * param, char status);

void isiPort(paramThread * param, int port);

void isiAddress(paramThread * param, char * address);


#ifdef _WIN32
	DWORD WINAPI serverSocket(LPVOID vParamT);

	DWORD WINAPI clientSocket(LPVOID vParamT);

	DWORD WINAPI repairingServer(LPVOID vParamT);

	DWORD WINAPI sendMessage (LPVOID client);

	DWORD WINAPI getMessage (LPVOID client);

#else
	void * serverSocket(void * vParamT);

	void * clientSocket(void * vParamT);

	void * repairingServer(void * vParamT);

	void * sendMessage (void * client);

	void * getMessage (void * client);
#endif
