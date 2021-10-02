#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include<time.h>
#include<sys/timeb.h>
#pragma comment(lib, "wsock32.lib")
#define BUF_SIZE 1024
void ErrorHandling(char* message);
long getSystemTime();

int main(int argc, char* argv[]) {
	WSADATA wsa_data;
	SOCKET hSocket;
	SOCKADDR_IN servAddr;
	char message[BUF_SIZE];
	
	int strLen;
	int count = 0;
	//if (argc != 3) {
	//	printf("Usage: %s <IP> <port>\n", argv[0]);
	//	exit(1);
	//}
	//WSAStartup(0x101, &wsa_data);
	if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0)
		ErrorHandling((char*)"WSAStartup() error");

	// TCP
	hSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (hSocket == INVALID_SOCKET)
		ErrorHandling((char*)"socket() error");

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr("114.55.252.36");
	//servAddr.sin_addr.S_un.S_addr = htonl(INADDR_LOOPBACK);
	servAddr.sin_port = htons(atoi("3000"));
	//servAddr.sin_port = htons(0x1234);

	if (connect(hSocket, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
		ErrorHandling((char*)"connect() error");
	else
	{
		puts("Connecting...");
	}

	while (1) {
		fputs("Input message(Q to quit): ", stdout);
		fgets(message, BUF_SIZE, stdin);

		if (!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
			break;
		long startTime = getSystemTime();
		send(hSocket, message, strlen(message), 0);
		strLen = recv(hSocket, message, BUF_SIZE - 1, 0);
		long endTime = getSystemTime();
		long time = endTime - startTime;
		message[strLen] = 0;
		printf("Message from server: %suse: %ld ms\n", message, time);
	}

	closesocket(hSocket);
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

