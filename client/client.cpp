#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include<time.h>
#include<sys/timeb.h>
#pragma comment(lib, "wsock32.lib")
void ErrorHandling(char* message);
long getSystemTime();

int main(int argc, char* argv[]) {
	SOCKET s;
	WSADATA wsa_data;
	SOCKADDR_IN servAddr;

	char message[30];
	int strLen;
	int count = 0;
	if (argc != 3) {
		printf("Usage: %s <IP> <port>\n", argv[0]);
		exit(1);
	}
	//WSAStartup(0x101, &wsa_data);
	if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0)
		ErrorHandling((char*)"WSAStartup() error");

	// UDP
	s = socket(AF_INET, SOCK_DGRAM, 0);
	if (s == INVALID_SOCKET)
		ErrorHandling((char*)"socket() error");

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(argv[1]);
	servAddr.sin_port = htons(atoi(argv[2]));

	if (connect(s, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
		ErrorHandling((char*)"connect() error");

	while (count < 30) {
		char mes[30] = "hello world111";
		long startTime = getSystemTime();
		send(s, mes, sizeof(mes) - 1, 0);
		strLen = recv(s, message, sizeof(message) - 1, 0);
		if (strLen == -1)
			// ErrorHandling((char*)"read error");
			continue;
		long endTime = getSystemTime();
		long time = endTime - startTime;
		printf("network time: %d \n", time);
		count++;
	}

	closesocket(s);
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

