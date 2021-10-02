#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include<time.h>
#include<sys/timeb.h>
#define BUF_SIZE 1024
#pragma comment(lib, "wsock32.lib")
void ErrorHandling(char* message);
long getSystemTime();

int main(int argc, char* argv[]) {
	int serv_sock, clnt_sock;

	char message[BUF_SIZE];
	int str_len, i;

	struct sockaddr_in serv_adr, clnt_adr;
	socklen_t clnt_adr_sz;
	//char message[] = "Hello World!";
	int count = 0;

	if (argc != 2) {
		printf("Usage: %s <IP> <port>\n", argv[0]);
		exit(1);
	}
	
	// TCP
	serv_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (serv_sock == -1)
		ErrorHandling((char*)"socket() error");

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(atoi(argv[1]));
	// serv_adr.sin_port = htons(0x1234);

	if (bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
		ErrorHandling((char*)"bind() error");

	if (listen(serv_sock, 5) == -1)
		ErrorHandling((char*)"listen() error");

	clnt_adr_sz = sizeof(clnt_adr);

	while (count < 30) {
		clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);
		if (clnt_sock == -1)
			ErrorHandling((char*)"accept() error");
		else
			printf("Connected client %d\n", count + 1);
		while ((str_len = read(clnt_sock, message, BUF_SIZE)) != 0)
		{
			write(clnt_sock, message, str_len);
		}
		close(clnt_sock);
		count++;
	}

	close(serv_sock);
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

