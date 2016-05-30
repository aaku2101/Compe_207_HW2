/* 
Name: Shah Aakash 
SJSU ID: 010767354
Class Id: 64
*/

#include<stdlib.h>
#include<stdio.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<pthread.h>
#include<string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>

int n;
void error(char *msg) 
{
	perror(msg);
	exit(1); //for abnormal termination

}

void *repeat(void *sockfd) 
{
		int sock = *(int *) sockfd;
		char buffer[256];

		bzero(buffer, 256); // to clear all previous buffer junk values

		n = read(sock, buffer, 255);

		if (n < 0) 
			perror("ERROR reading from socket");

		printf("Message from Client is: %s\n", buffer);

		n = write(sock, "Server got your message", 25);

		if (n < 0) 
			perror("ERROR writing to socket");

}

int main(int argc, char *argv[]) 
{
	int sockfd, newsockfd, portno; 
	socklen_t clilen;
	int *newsock;

	struct sockaddr_in serv_addr, cli_addr;

	if (argc < 2) 
	{
		fprintf(stderr, "No port is provided,ERROR\n");
		exit(1);
	}

	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	bzero((char *) &serv_addr, sizeof(serv_addr)); // to clear all previous server addresses and junk values

	portno = atoi(argv[1]);

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);

	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
	{
		perror("ERROR on Binding");
	}

	listen(sockfd, 5); // upto 5 clients server can listen
	clilen = sizeof(cli_addr);

	while (newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen)) 
	{
		pthread_t sniff;
		newsock = (int *)malloc(1);
		*newsock = newsockfd;
		if (pthread_create(&sniff, NULL, repeat, (void *) newsock)< 0) 
		{
			perror("ERROR in creating thread");
		}

		//printf(" Handler assigned ");

		//return 0;
	}
	close (sockfd);
}

