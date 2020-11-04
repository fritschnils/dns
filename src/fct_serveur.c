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

int init_socket(struct sockaddr_in6 *address, long int port, const char *txt_addr, int addrlen, int is_recv_socket)
{
	int sockfd, ip_bin;
	if((sockfd = socket(PF_INET6, SOCK_DGRAM, IPPROTO_UDP)) == -1)
		raler("socket", 1);

	address->sin6_family = AF_INET6;
	address->sin6_port = htons(port);
	ip_bin = inet_pton(AF_INET6, txt_addr, &address->sin6_addr);
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

/*              3. An   alternate  format  is  useful  for  expressing  IPv4-mapped  IPv6
                 addresses.  This form is written as x:x:x:x:x:x:d.d.d.d, where the six
                 leading xs are hexadecimal values that define the six most-significant
                 16-bit pieces of the address (i.e., 96 bits), and  the  ds  express  a
                 value in dotted-decimal notation that defines the least significant 32
                 bits  of  the  address.   An   example   of   such   an   address   is
                 ::FFFF:204.152.189.116.
*/