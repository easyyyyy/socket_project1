// 连接客户.cpp : 与服务器建立连接后，向服务器发送一个字符串，收到服务器应答后，关闭连接。
//
#include <winsock.h>
#include <stdio.h>
#pragma comment(lib,"wsock32.lib")
int main(int argc, char* argv[])
{
	SOCKET s;
	sockaddr_in local, server;
	WSAData wsa;
	char buf[] = "I am a client";
	char recvbuf[128];

	WSAStartup(0x101, &wsa);

	s = socket(AF_INET, SOCK_STREAM, 0);
	local.sin_family = AF_INET;
	local.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	local.sin_port = htons(0x3412);

	bind(s, (sockaddr*)&local, sizeof(local));

	server.sin_family = AF_INET;
	server.sin_addr.S_un.S_addr = htonl(INADDR_LOOPBACK);
	server.sin_port = htons(0x1234);

	connect(s, (sockaddr*)&server, sizeof(server));

	send(s, buf, strlen(buf) + 1, 0);//strlen不包含字符串结束符\0
	recv(s, recvbuf, sizeof(recvbuf), 0);
	printf("Message from server: %s \n", recvbuf);
	closesocket(s);
	WSACleanup();
	system("pause");
	return 0;
}

