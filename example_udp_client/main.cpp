// connless.cpp : 向服务器发送一个字符串后，等到服务器的应答，结束
//

#include <winsock2.h>
#include <stdio.h>
#pragma comment(lib,"ws2_32.lib")
int main(int argc, char* argv[])
{
	int len;
	char sndbuf[] = "I am a cl client";
	char recvbuf[128];
	SOCKET s;
	SOCKADDR_IN local, remote;
	WSAData wsa;

	WSAStartup(0x101, &wsa);
	s = socket(AF_INET, SOCK_DGRAM, 0);

	local.sin_family = AF_INET;
	local.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	local.sin_port = htons(0x3412);

	bind(s, (SOCKADDR*)&local, sizeof(local));

	remote.sin_family = AF_INET;
	remote.sin_addr.S_un.S_addr = htonl(INADDR_LOOPBACK);
	remote.sin_port = htons(0x1234);

	len = sizeof(remote);
	sendto(s, sndbuf, 17, 0, (SOCKADDR*)&remote, len);

	len = sizeof(remote);
	recvfrom(s, recvbuf, sizeof(recvbuf), 0, (SOCKADDR*)&remote, &len);
	printf("Message from server: %s \n", recvbuf);
	closesocket(s);
	WSACleanup();
	return 0;
}
