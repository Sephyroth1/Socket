#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>


int main(int argc, char *argv[]) {

	WSADATA wsaData;
	int iResult;
	int attach, listenfd, backlog = 3, acceptfd;
	SOCKET sockfd;

	/*int receivefd, len = 100;
	char buff[100];*/
	
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed: %d\n", iResult);
		return 1;
	}

	struct addrinfo serv;

	char clientIp[40];

	memset(&serv, 0, sizeof(serv));

	serv.ai_family = AF_INET;
	serv.ai_protocol = IPPROTO_TCP;
	serv.ai_socktype = SOCK_STREAM;


	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if (sockfd < 0) {
		fprintf(stderr, "socket error: %s\n", gai_strerror(sockfd));
		WSACleanup();
		return 1;
	}
	struct sockaddr_in addr;
	socklen_t bytes = sizeof(addr);
	memset(addr.sin_zero, '\0', sizeof(addr.sin_zero));
	addr.sin_family  = AF_INET;
	addr.sin_port = htons(8080);
	inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);


	attach = bind(sockfd, (struct sockaddr *)&addr, bytes);
	if (attach < 0) {
		perror("bind error");
		fprintf(stderr, "%d \n", errno);
		WSACleanup();
		return 1;
	}

	printf("Socket Bound successfully\n");
	listenfd = listen(sockfd, backlog);
	if (listenfd != 0) {
		perror("listen: ");
		WSACleanup();
		return 1;
	}

	socklen_t addr_size = sizeof(addr);
	acceptfd = accept(sockfd, (struct sockaddr *)&addr, &addr_size);

	if (acceptfd == -1) {
		printf("%d \n", acceptfd);
		perror("accept got the error");
		printf("%d: errno, %d: acceptfd", errno, acceptfd);
		closesocket(sockfd);
		WSACleanup();
	}

	char *Hello = "Hello World";
	send(acceptfd, Hello, strlen(Hello), 0);

	inet_ntop(AF_INET, &(addr.sin_addr), clientIp, sizeof(clientIp));
	printf("Connection accepted from the client: %s\n %d\n", clientIp, addr.sin_port);
	
	char buffer1[1024];

	int bytes_send = recv(acceptfd, buffer1, strlen(buffer1), 0);
	if(bytes_send == -1){
		printf("%d \n", bytes_send);
		perror("recv");
		WSACleanup();
	}else if(bytes_send == 0){
		printf("Connection was closed by client\n");
	}else{
		printf("Data Received successfully\n");
		printf("%s = received data", buffer1);
	}

	closesocket(sockfd);
	WSACleanup();
	return 0;
}