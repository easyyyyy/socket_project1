// connless.cpp : 从客户机收到数据后，回传一个ACK字符串.
//
#include <winsock2.h>
#include <stdio.h>
#pragma comment(lib,"ws2_32.lib")
int main(int argc, char* argv[])
{
	SOCKET s;
	sockaddr_in local;
	sockaddr_in remote;
	int len;
	char buf[128];
	WSAData wsa;

	WSAStartup(0x101, &wsa);
	s = socket(AF_INET, SOCK_DGRAM, 0);
	local.sin_family = AF_INET;
	local.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	local.sin_port = htons(0x1234);

	bind(s, (sockaddr*)&local, sizeof(local));
	while (1) {
		len = sizeof(remote);
		recvfrom(s, buf, sizeof(buf), 0, (sockaddr*)&remote, &len);
		printf("Message from client: %s \n", buf);
		len = sizeof(remote);
		sendto(s, "ACK", 4, 0, (sockaddr*)&remote, len);

	}
	closesocket(s);
	WSACleanup();
	return 0;
}
