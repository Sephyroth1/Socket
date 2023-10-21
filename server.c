#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>


int main(int argc, char *argv[]){

	WSADATA wsaData;
	int iResult, status;
	int attach, listenfd, backlog = 10, acceptfd;
	SOCKET sockfd;
	/*int receivefd, len = 100;
	char buff[100];*/

	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed: %d\n", iResult);
        return 1;
    }

	struct addrinfo serv, *info = NULL;

	char clientIp[40];
	char *name = "127.0.0.1", *type = "http";

	memset(&serv, 0, sizeof(serv));

	serv.ai_family = AF_INET;
	serv.ai_protocol = IPPROTO_TCP;
	serv.ai_socktype = SOCK_STREAM;

	status = getaddrinfo(name, type, &serv, &info);
	if (status != 0){
		fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
		WSACleanup();
		return 1;
	}

	sockfd = socket(info->ai_family, info->ai_socktype, info->ai_protocol);

	if(sockfd < 0){
		fprintf(stderr, "socket error: %s\n", gai_strerror(sockfd));
		WSACleanup();
		return 1;
	}
	struct sockaddr_in addr, claddr;
	socklen_t bytes = sizeof(struct sockaddr_in);
	memset(addr.sin_zero, '\0', sizeof(addr.sin_zero));
	addr.sin_family  = AF_INET;
	addr.sin_port = htons(8080);
	inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);

	memset(claddr.sin_zero, '\0', sizeof(claddr.sin_zero));
	claddr.sin_family = AF_INET;
	claddr.sin_port = htons(8080);
	inet_pton(AF_INET, "127.0.0.1", &claddr.sin_addr);

	attach = bind(sockfd, info->ai_addr, bytes);
	if(attach < 0){
		perror("bind error");
		fprintf(stderr, "%d \n", errno);
		WSACleanup();
		return 1;
	}

	printf("Socket Bound successfully");
	listenfd = listen(sockfd, backlog);
	if(listenfd != 0){
		perror("listen: ");
		WSACleanup();
		return 1;
	}
	//clientsocket = socket(AF_INET, SOCK_STREAM, 0);

	socklen_t addr_size = sizeof(struct sockaddr_in);
	acceptfd = accept(sockfd, (struct sockaddr *)&claddr, &addr_size);

	if(acceptfd == -1){
		printf("%d \n", acceptfd);
		perror("accept got the error");
		printf("%d: errno, %d: acceptfd", errno, acceptfd);
		closesocket(sockfd);
		WSACleanup();
	}

	inet_ntop(AF_INET, &(claddr.sin_addr), clientIp, sizeof(clientIp));
	printf("Connection accepted from the client: %s\n%d", clientIp, claddr.sin_port);

	closesocket(sockfd);
	freeaddrinfo(info);
	WSACleanup();
	return 0;
}