#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int main(int argc, char *argv[]){

	WSADATA wsaData; 	
	int iResult;
	int sockfd, connectfd;
	char buffer[1024];

	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed: %d\n", iResult);
        return 1;
    }

	sockfd = socket(AF_INET, SOCK_STREAM, 0);

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

	connectfd = connect(sockfd, (struct sockaddr *)&addr, bytes);
	if(connectfd == -1){
		perror("connect error");
		fprintf(stderr, "error: %d and connectfd: %d", errno, connectfd);
		WSACleanup();
		return -1;
	}
	printf("%d is the error_code\n", connectfd);
	printf("Socket connected successfully\n");

	int bytes_send = recv(sockfd, buffer, sizeof(buffer), 0);
	if(bytes_send == -1){
		printf("%d \n", bytes_send);
		perror("recv");
		WSACleanup();

	}else if(bytes_send == 0){
		printf("Connected was closed by server\n");
	}else{
		printf("Data Received Successfully\n");
		printf("%s = received data", buffer);
	}

	char *buffers = "Hello World to you too!";
	send(sockfd, buffers, strlen(buffers), 0);
	closesocket(sockfd);
	WSACleanup();
	return 0;
}