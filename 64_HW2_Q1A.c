/* 
Name: Shah Aakash 
SJSU ID: 010767354
Class Id: 64
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>

#include <string.h>

void process (int sock);

int main(int argc, char **argv) 
{
   int sockfd, newsockfd;
   socklen_t clilen;
   char buffer[1024];
   struct sockaddr_in serv_addr, cli_addr;
   int n, pid;
   
   if (argc != 2)
   {
      fprintf(stderr,"usage %s port\n", argv[0]);
      exit(1);
   }

   int portno = atoi(argv[1]);   
   sockfd = socket(AF_INET, SOCK_STREAM, 0);  //First call to socket() function

   bzero(&serv_addr,sizeof(serv_addr));  //Initialize socket structure
   
   serv_addr.sin_family = AF_INET;
   serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
   serv_addr.sin_port = htons(portno);
   
   if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)  //Now bind the host address using bind() 
   {
      perror("ERROR on binding");
      exit(1);
   }
   
   listen(sockfd,5);  //Now start listening for the clients
   clilen = sizeof(cli_addr);
   
   while (1) 
   {
      newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
		
      if (newsockfd < 0) {
        perror("ERROR in accept");
        exit(1);
      }
      
      pid = fork();  //Create child process 
		
      if (pid < 0) {
        perror("ERROR in fork");
        exit(1);
      }
      
      if (pid == 0)  //This is the client process 
	  {
        close(sockfd);
        process(newsockfd);
        exit(0);
      }
      else 
	  {
        close(newsockfd);
      }
		
    } 
}

void process (int sock) 
{
   int n;
   char buffer[1024];
   bzero(buffer,1024);
   n = read(sock,buffer,1024);
 
   printf("Here is the message: %s\n",buffer);
   n = write(sock,"I got your message",18);
}
