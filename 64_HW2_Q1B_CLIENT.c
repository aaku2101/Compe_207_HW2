/* 
Name: Shah Aakash 
SJSU ID: 010767354
Class Id: 64
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>

#define ERROR -1

int main(int argc, char const *argv[])
{
	
	//start the server process 1- step is to make a socket
	//socket creation and error checking 
	struct sockaddr_in remote_server; //remote_srever if  of type sockaddr_in , 
	                           //this contains the port and ip address of the remote tcp echo server which we want to connect
	int sockfd; 				//socket descriptor used to connect to the remote server
	char input[1024];   	//user input being stored in this buffer
	char output[1024]; 	//what basically sent form remote server
	unsigned int length;

	//TCP socket creation, to connect to the remote server and error checking 
	if ((sockfd = socket(AF_INET, SOCK_STREAM,0)) == ERROR) {  //SOCK_STREAM means TCP 
		perror("serevr socket: ");
		exit(-1);
	}

	//remote server data structure/details in sockaddr_in structure

	remote_server.sin_family = AF_INET;
	remote_server.sin_port = atoi(argv[1]);  //port number
											//convert to ascii to integer , then host byte order to network byte order
	remote_server.sin_addr.s_addr = INADDR_ANY;  //ip address , converted
	bzero(&remote_server.sin_zero, 8);   		//padding

	//next step -connect , error check

	if ((connect(sockfd, (struct sockaddr *)&remote_server, sizeof(struct sockaddr_in))) == ERROR) {
		perror("connect");
		exit(-1);
	}

	while(1) 
	{   //main loop

		fgets(input, 1024, stdin);  //fgets to get some input for the user, 
		send(sockfd, input, strlen(input),0);	//basically line which user inputs for the console, we send that line asis to remote server
		length = recv(sockfd, output, 1024, 0); // As the echo server sends the same input , so to receive we use receive call
		output[length] = '\0';
		printf("%s\n",output);  			// we display that to the user
	}
		close(sockfd);
	return 0;  // we need to press ctrl + c which is going to exit the program 
}
