/* Name : Aakash Shah
SJSU ID: 010767354#include*/

#include<stdio.h>
#include<stdlib.h>
#include<strings.h>
#include<string.h>
#include<netinet/in.h>
#include<unistd.h>
#include<netdb.h>
#include<sys/types.h>
#include<sys/socket.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>
#include <arpa/inet.h>

#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef	INADDR_NONE
#define	INADDR_NONE	0xffffffff
#endif	/* INADDR_NONE */

int	connectsock(const char *host, const char *service, const char *transport);
int	errexit(const char *format, ...);
int	connectTCP(const char *host, const char *service);

int main(int argc, char *argv[])
{
	int sockfd, newsockfd, n;
	struct sockaddr_in server;
	char buf[1024];

	char	*host = "localhost";
	char	*service = "time";
	
	sockfd = connectTCP(host, service);
	
	if(sockfd < 0)
	{
		perror("Socket Failed");
		exit(1);
	}
		
	printf("Enter the message\n");
	printf("CLient:");
	
	while(fgets(buf, 1023, stdin))
	{	
		n = write(sockfd, buf, strlen(buf));
		if(n < 0)
			perror("Cannot Write message");
		
		bzero(buf, 1024);
		n = read(sockfd, buf, 1023);
		if(n < 0)
			perror("Cannot Read message");

		printf("Server: %s\n",buf);
		
	}

	close(sockfd);
	return 0;
}

int connectTCP(const char *host, const char *service )
/*
 * Arguments:
 *      host    - name of host to which connection is desired
 *      service - service associated with the desired port
 */
{
	return connectsock( host, service, "tcp");
}

int connectsock(const char *host, const char *service, const char *transport )
/*
 * Arguments:
 *      host      - name of host to which connection is desired
 *      service   - service associated with the desired port
 *      transport - name of transport protocol to use ("tcp" or "udp")
 */
{
	struct hostent	*phe;	/* pointer to host information entry	*/
	struct servent	*pse;	/* pointer to service information entry	*/
	struct protoent *ppe;	/* pointer to protocol information entry*/
	struct sockaddr_in sin;	/* an Internet endpoint address		*/
	int	s, type;	/* socket descriptor and socket type	*/


	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;

    /* Map service name to port number */
	if ( pse = getservbyname(service, transport) )
		sin.sin_port = pse->s_port;
	else if ((sin.sin_port=htons((unsigned short)atoi(service))) == 0)
		errexit("can't get \"%s\" service entry\n", service);

    /* Map host name to IP address, allowing for dotted decimal */
	if ( phe = gethostbyname(host) )
		memcpy(&sin.sin_addr, phe->h_addr, phe->h_length);
	else if ( (sin.sin_addr.s_addr = inet_addr(host)) == INADDR_NONE )
		errexit("can't get \"%s\" host entry\n", host);

    /* Map transport protocol name to protocol number */
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

    /* Connect the socket */
	if (connect(s, (struct sockaddr *)&sin, sizeof(sin)) < 0)
		errexit("can't connect to %s.%s: %s\n", host, service,
			strerror(errno));
	return s;
}

int errexit(const char *format, ...)
{
	va_list	args;

	va_start(args, format);
	vfprintf(stderr, format, args);
	va_end(args);
	exit(1);
}
