/* 
Name: Shah Aakash 
SJSU ID: 010767354
Class Id: 64
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
	
#define PORT 777
#define BUFSIZE 1024

	
void send_recv(int newSocket, int welcomeSocket, fd_set *master, int fdmax)
{
	int nbytes_recvd;
	char recv_buf[BUFSIZE];
	char message[50] = "Data Received at Server";
	
	bzero(recv_buf,BUFSIZE);
	if ((nbytes_recvd = read(newSocket, recv_buf, BUFSIZE)) <= 0) {
		if (nbytes_recvd == 0) {
			printf("socket %d disconnected\n", newSocket);
		}else {
			perror("recv");
		}
		close(newSocket);
		FD_CLR(newSocket, master);
	}else { 
		printf("Data Received from Socket %d: %s\n",newSocket,recv_buf);
		write(newSocket,message,strlen(message));
		bzero(recv_buf,BUFSIZE);
	}	
}
		
void connection_accept(fd_set *master, int *fdmax, int welcomeSocket, struct sockaddr_in *client_addr)
{
	socklen_t addrlen;
	int newSocket;
	
	addrlen = sizeof(struct sockaddr_in);
	if((newSocket = accept(welcomeSocket, (struct sockaddr *)client_addr, &addrlen)) == -1) {
		perror("accept");
		exit(1);
	}else {
		FD_SET(newSocket, master);
		if(newSocket > *fdmax){
			*fdmax = newSocket;
		}
		printf("New connection from %s on port %d \n",inet_ntoa(client_addr->sin_addr), ntohs(client_addr->sin_port));
	}
}
	
void connect_request(int *welcomeSocket, struct sockaddr_in *server_addr)
{
	int yes = 1;
		
	if ((*welcomeSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("Socket");
		exit(1);
	}
		
	server_addr->sin_family = AF_INET;
	server_addr->sin_port = 777;
	server_addr->sin_addr.s_addr = INADDR_ANY;
	memset(server_addr->sin_zero, '\0', sizeof server_addr->sin_zero);
		
	if (setsockopt(*welcomeSocket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
		perror("setsockopt");
		exit(1);
	}
		
	if (bind(*welcomeSocket, (struct sockaddr *)server_addr, sizeof(struct sockaddr)) == -1) {
		perror("Unable to bind");
		exit(1);
	}
	if (listen(*welcomeSocket, 10) == -1) {
		perror("listen");
		exit(1);
	}
	printf("\nconnected to server on 777\n");
	fflush(stdout);
}
int main()
{
	fd_set master;
	fd_set read_fds;
	int fdmax, index;
	int welcomeSocket= 0;
	struct sockaddr_in server_addr, client_addr;
	FD_ZERO(&master);
	FD_ZERO(&read_fds);
	connect_request(&welcomeSocket, &server_addr);
	FD_SET(welcomeSocket, &read_fds);
	FD_SET(welcomeSocket, &master);
	
	fdmax = welcomeSocket;
	
	while(1){
		read_fds = master;
		if(select(fdmax+1, &read_fds, NULL, NULL, NULL) == -1){
			perror("select");
			exit(4);
		}
		
		for (index = 0; index <= fdmax; index++){
			if (FD_ISSET(index, &read_fds)){
				if (welcomeSocket == index)
					connection_accept(&master, &fdmax, welcomeSocket, &client_addr);
				else
					send_recv(index,welcomeSocket, &master,fdmax);
			}
		}
	}
	return 0;
}
	
