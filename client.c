#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>


int main(int argc, char *argv[]){

	WSADATA wsaData; 	
	int iResult, status;
	int sockfd, connectfd;
	//char buffer[70];

	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed: %d\n", iResult);
        return 1;
    }

	struct addrinfo serv, *info = NULL;

	char ipbuffer[40];
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

	void *addr1;

	printf("Reached point A\n");

	struct sockaddr_in *ipv4 = (struct sockaddr_in * )info->ai_addr;
	addr1 = &(ipv4->sin_addr);
	inet_ntop(info->ai_family, addr1, ipbuffer, sizeof(ipbuffer));

	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	printf("Reached point B\n");

	if(sockfd < 0){
		perror("socket");
		WSACleanup();
		return 1;
	}
	struct sockaddr_in addr;
	memset(&addr, '0', sizeof(addr));
	addr.sin_family  = AF_INET;
	addr.sin_port = htons(8080);
	inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);
	socklen_t bytes = sizeof(addr);
	printf("Reached point C\n");

	connectfd = connect(sockfd, (struct sockaddr *)&addr, bytes);
	if(connectfd == -1){
		perror("connect error");
		fprintf(stderr, "error: %d and connectfd: %d", errno, connectfd);
		WSACleanup();
		return -1;
	}
	printf("%d \n", connectfd);
	printf("Socket connected successfully\n");

	/*int bytes = recv(sockfd, buffer, sizeof(buffer), 0);
	if(bytes == -1){
		printf("%d \n", bytes);
		perror("recv");
		WSACleanup();

	}else if(bytes == 0){
		printf("Connected was closed by server");
	}else{
		printf("Data Received Successfully");
		printf("%s\n = received data", buffer);
	}*/
	closesocket(sockfd);
	freeaddrinfo(info);
	WSACleanup();
	return 0;
}