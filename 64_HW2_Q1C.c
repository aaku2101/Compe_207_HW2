/* Name : Aakash Shah
SJSU ID: 0101767354
*/
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <fcntl.h>
//#include "passivesock.h"
//#include "passiveTCP.h"
//#include "errexit.h"

#define QLEN 32

typedef unsigned long u_long;

int errexit(const char *format, ...);
int passiveTCP(const char *service, int qlen);
int	passivesock(const char *service, const char *transport, int qlen);
void SigFunc(int signo);

int newsockfd, n;
char buf[1024];

int main(int argc, char *argv[]) 
{
	char *service = "time";
	struct sockaddr_in server;	//the from address of a client  
	int alen;			// from-address length 
	int sockfd;
	int flag;
	
	sockfd= passiveTCP(service, QLEN);
	
	while (1) 
	{
		alen = sizeof(server);
		newsockfd = accept(sockfd, (struct sockaddr *)&server, &alen);
		if(newsockfd < 0)
			errexit("accept: %s\n", strerror(errno));
			
		if(signal(SIGIO, SigFunc) == SIG_ERR)
			printf("cannot catch signal SIGIO\n");

		if(fcntl(sockfd, F_SETOWN, getpid()) < 0) {
			perror("fcntl F_SETOWN");
			//exit(1);
		}
		
		flag = fcntl(sockfd, F_GETFL);
		if(fcntl(sockfd, F_SETFL, flag|O_ASYNC) < 0) {
			perror("fcntl F_SETFL, ASYNC");
			//exit(1);
		}

		(void) close(newsockfd);
	}
}

void SigFunc(int signo) 
{
	if(signo == SIGIO)
		printf("signal received\n");
	memset(buf, 0, 1024);
	n = read(newsockfd, buf, 1023);
	if(n < 0)
		perror("Cannot read the message from Client");
			
	else if(n == 0)
		printf("Connection End");		
			
	printf("Client: %s\n", buf);
			
	n = write(newsockfd, "Message Received", 16);
	if(n<0)
		perror("Error in sending message");
}

int passivesock(const char *service, const char *transport, int qlen)
/*
 * Arguments:
 *      service   - service associated with the desired port
 *      transport - transport protocol to use ("tcp" or "udp")
 *      qlen      - maximum server request queue length
 */
{
	struct servent	*pse;	/* pointer to service information entry	*/
	struct protoent *ppe;	/* pointer to protocol information entry*/
	struct sockaddr_in sin;	/* an Internet endpoint address		*/
	int	s, type, portbase;	/* socket descriptor and socket type	*/

	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = INADDR_ANY;

    /* Map service name to port number */
	if ( pse = getservbyname(service, transport) )
		sin.sin_port = htons(ntohs((unsigned short)pse->s_port)
			+ portbase);
	else if ((sin.sin_port=htons((unsigned short)atoi(service))) == 0)
		errexit("can't get \"%s\" service entry\n", service);

    /* Map protocol name to protocol number */
	if ( (ppe = getprotobyname(transport)) == 0)
		errexit("can't get \"%s\" protocol entry\n", transport);

    /* Use protocol to choose a socket type */
	if (strcmp(transport, "udp") == 0)
		type = SOCK_DGRAM;
	else
		type = SOCK_STREAM;

    /* Allocate a socket */
	s = socket(PF_INET, type, ppe->p_proto);
	if (s < 0)
		errexit("can't create socket: %s\n", strerror(errno));

    /* Bind the socket */
	if (bind(s, (struct sockaddr *)&sin, sizeof(sin)) < 0)
		errexit("can't bind to %s port: %s\n", service,
			strerror(errno));
	if (type == SOCK_STREAM && listen(s, qlen) < 0)
		errexit("can't listen on %s port: %s\n", service,
			strerror(errno));
	return s;
}


int passiveTCP(const char *service, int qlen)
/*
 * Arguments:
 *      service - service associated with the desired port
 *      qlen    - maximum server request queue length
 */
{
	return passivesock(service, "tcp", qlen);
}

int errexit(const char *format, ...)
{
	va_list	args;

	va_start(args, format);
	vfprintf(stderr, format, args);
	va_end(args);
	exit(1);
}
