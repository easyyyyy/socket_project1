#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock.h>
#include<time.h>
#include<sys/timeb.h>
#pragma comment(lib, "wsock32.lib")
void ErrorHandling(char* message);
long getSystemTime();

int main(int argc, char* argv[]) {
	SOCKET hServSock, hClntSock;
	WSADATA wsa_data;
	SOCKADDR_IN servAddr, clntAddr;

	int szClntAddr;
	char message[] = "Hello World!";
	int count = 0;
	if (argc != 2) {
		printf("Usage: %s <IP> <port>\n", argv[0]);
		exit(1);
	}
	//WSAStartup(0x101, &wsa_data);
	if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0)
		ErrorHandling((char*)"WSAStartup() error");

	// UDP
	hServSock = socket(AF_INET, SOCK_STREAM, 0);
	if (hServSock == INVALID_SOCKET)
		ErrorHandling((char*)"socket() error");

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(atoi(argv[1]));

	if (bind(hServSock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
		ErrorHandling((char*)"bind() error");

	if (listen(hServSock, 5) == SOCKET_ERROR)
		ErrorHandling((char*)"listen() error");

	while (count < 30) {
		szClntAddr = sizeof(clntAddr);
		hClntSock = accept(hServSock, (SOCKADDR*)&clntAddr, &szClntAddr);
		if (hClntSock == INVALID_SOCKET)
			ErrorHandling((char*)"accept() error");
		send(hClntSock, message, sizeof(message), 0);
		count++;
	}

	closesocket(hServSock);
	closesocket(hClntSock);
	WSACleanup();
	return 0;
}

void ErrorHandling(char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

long getSystemTime() {
	struct timeb tb;
	ftime(&tb);
	return tb.time * 1000 + tb.millitm;//time单位是秒，millitm单位是毫秒
}

