#include <sys/types.h>	
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <limits.h>
#include <string.h>


void raler(char *msg, int perror_isset)
{
	fprintf(stderr, "%s\n", msg);
	if (perror_isset)
		perror(msg);
	exit(EXIT_FAILURE);
}

int init_socket(struct sockaddr_in *address, long int port, const char *txt_addr, int addrlen, int is_recv_socket)
{
	int sockfd, ip_bin;
	if((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
		raler("socket", 1);

	address->sin_family = AF_INET;
	address->sin_port = htons(port);
	ip_bin = inet_pton(AF_INET, txt_addr, &address->sin_addr);
	if(ip_bin == 0)
		raler("inet_pton : src does not contain a character string representing a valid network address in the specified address family", 0);
	if(ip_bin == -1)
		raler("inet_pton", 1);

	if(is_recv_socket){
		if((bind(sockfd, (struct sockaddr *) address, addrlen)) == -1)
			raler("bind", 1);
	}

	return sockfd;
}