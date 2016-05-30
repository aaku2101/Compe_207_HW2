/* 
Name: Shah Aakash 
SJSU ID: 010767354
Class Id: 64
*/

/* Explaination of the code:
##########################################################################################################################################
	TCPDaytimeServer the master process of the server will create a new child process (slave) to handle each incoming request.  
	The slave process will convert the time string into the following format:MM/DD/YYYY HH:MM:SS Weekday for example, 
	“02/26/2015 22:01:02 Thu”.  The slave process will then send the revised time string back to the client.
############################################################################################################################################*/

#include<sys/types.h>
#include<stdlib.h>
#include<signal.h>
#include<sys/socket.h>
#include<unistd.h>
#include<stdio.h>
#include<string.h>
#include<time.h>
#include<sys/signal.h>
#include<sys/resource.h>
#include<sys/errno.h>
#include<netinet/in.h>
#include<wait.h>
#include<netdb.h>


int TCPdaytimed(int fd);
int passiveTCP(const char *service, const char *transport, int qlen);
void signalHandler(int signal);
#define ZERO 0
#define QLEN 32
#define ERROR -1
#define DATA_SIZE 1024
#define DEFAULT_TCP_SERVICE_PORT 5000

int main(int argc, char *argv[])
{
	struct sockaddr_in fsin; /* the from address of a client */
	char *service = "daytime"; /* service name or port number */
	char *transport = "tcp";
	int msock, ssock;         /* master and slave sockets */
	unsigned int alen;	  /* from address len */
	void sig_chld(int);
	int child_pid;
	switch(argc) 
	{

		case 1:
			break;
		case 2:
			service = argv[1];
			break;
		case 3:
			service = argv[1];
			transport = argv[2];

			break;
		default:
			printf("usage: TCPdaytimed [port] [tcp] \n");

	}
	msock = passiveTCP(service,transport,QLEN);
	printf("Socket Created Successfully ... \n");
	(void)signal(SIGCHLD, signalHandler); /*must call waitpid*/
	while(1)
	{	
		alen = sizeof(fsin);
		ssock = accept(msock, (struct sockaddr *)&fsin, &alen);
		if((child_pid = fork()) == 0)
		{
			TCPdaytimed(ssock);
			close (ssock);
			printf("Client Disconnected .... \n");
			return 0;
		}
		close (ssock);
	//	close (msock);
	}
	close(msock);
}
 /* -----------------------------------------------------------------------*/
   	/* TCPdaytimed - do TCP daytime protocol */
 /*------------------------------------------------------------------------*/

int TCPdaytimed(int fd)
	{
		char *pts;
		struct tm time_conv;
		time_t now;
		char *ctime();
		char *timeData;
		int ret = 0;
		int bytes = 0, data;
		timeData = (char *)malloc(DATA_SIZE);
		//Current time is got here.
		time(&now);
		// Format time
		pts = ctime(&now);
		printf("The Current time at the Server side is %s\n", pts);
		bytes = send(fd,pts,DATA_SIZE,0);
		if (bytes < 0) {
			ret = ERROR;
		}
		printf("(SERVER) Data Sent : %s \n", pts);
		return ret;
	}

/*--------------------------------------------------------------------------*/
	/* PassiveTCPSocket - Creation of TCP Socket */
/*--------------------------------------------------------------------------*/

int passiveTCP(const char *service,const char *transport, int qlen)
{
        struct sockaddr_in serverConcurrent;
        struct servent *socketEntity = NULL;
        int sock_init;
        bzero(&serverConcurrent,sizeof(struct sockaddr_in));
        serverConcurrent.sin_family = AF_INET;
        serverConcurrent.sin_addr.s_addr = INADDR_ANY;
        /* Map service name to port number */

        socketEntity = getservbyname(service, transport);
        if (NULL != socketEntity) 
	{
                printf("Requested service '%s' is running on the host system on port %d.\n", service, socketEntity->s_port);
                serverConcurrent.sin_port = socketEntity->s_port;
                printf("Name: %s\nPort: %d\n", socketEntity->s_name, ntohs(socketEntity->s_port));
        }
        else if(ZERO != (serverConcurrent.sin_port = htons((unsigned short)atoi(service)))) 
	{
                printf("The specified service '%s' is a well-defined port number\n", service);
        } else 
	{
                printf("Using default service port '%d'\n", DEFAULT_TCP_SERVICE_PORT);
                serverConcurrent.sin_port = htons(DEFAULT_TCP_SERVICE_PORT);
        }

        /* Allocate a socket */
        sock_init = socket(AF_INET,SOCK_STREAM,0);
        if(sock_init == ERROR)
        {
                perror("Failed Creating Socket ......:");
                exit(ERROR);
        }
        /* Bind the socket */
        if(bind(sock_init, (struct sockaddr *)&serverConcurrent, sizeof(serverConcurrent)) < 0) 
	{
                perror("Bind Failed: Can't get service entry:");
                exit(ERROR);
        }
        listen(sock_init, QLEN);
        return sock_init;
}

void signalHandler(int signal)
{
        int status;
        int childPid=0;
        while ((childPid = wait3(&status, WNOHANG, (struct rusage *)0)) >= 0)
                printf("Child %d terminated\n", childPid);
}

