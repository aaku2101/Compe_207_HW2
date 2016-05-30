/* 
 * tcpclient.c - A simple TCP client
 * usage: tcpclient <host> <port>
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <arpa/inet.h>

/* 
 * error - wrapper for perror
 */
void error(char *msg) 
{
    perror(msg);
    exit(0);
}

int main(int argc, char **argv) 
{
    int sockfd, n;
    struct sockaddr_in serveraddr;
    struct hostent *server;
    //char *hostname;
    char buffer[1024];

    /* check command line arguments */
    if (argc != 3) 
    {
       fprintf(stderr,"usage: %s <hostname> <port>\n", argv[0]);
       exit(1);
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    bzero(&serveraddr, sizeof(serveraddr));

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr=inet_addr(argv[1]);
   
    int portno = atoi(argv[2]);
    
    serveraddr.sin_port = htons(portno);

    printf("Type something: ");
   
    while (fgets(buffer, 1024,stdin) != NULL)
    {
	connect(sockfd,(struct sockaddr *)&serveraddr,sizeof(serveraddr));
        n = write(sockfd, buffer, strlen(buffer));
        bzero(buffer, 1024);
        n = read(sockfd, buffer, 1024);
        printf("Echo from server: %s", buffer);
        bzero(buffer, 1024);
    }
close(sockfd);
return 0;
}
