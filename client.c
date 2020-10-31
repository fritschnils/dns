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


int main(int argc, char const *argv[])
{
	int sockfd_serveur, sockfd_client, ip_bin, nb_octets;
	long int port_serveur = 50764, port_client = 40764;
	char *ip_client = "127.0.0.1"; 
	char *sent_request = "salut";
	char buf[1024];

	socklen_t addrlen;
	struct sockaddr_in client;
	struct sockaddr_in dest;

	// SOCKET CLIENT 
	if((sockfd_client = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
		raler("socket", 1);

	client.sin_family = AF_INET;
	client.sin_port = htons(port_client);
	client.sin_addr.s_addr = htonl(INADDR_ANY);

	addrlen = sizeof(struct sockaddr_in);
	memset(buf, '\0', 1024);
	if((bind(sockfd_client, (struct sockaddr *) &client, addrlen)) == -1)
		raler("bind", 1);

	// SOCKET SERVEUR
	if((sockfd_serveur = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
		raler("socket", 1);

	dest.sin_family = AF_INET;
	dest.sin_port = htons(port_serveur);
	ip_bin = inet_pton(AF_INET, ip_client, &dest.sin_addr);
	if(ip_bin == 0)
		raler("inet_pton : src does not contain a character string representing a valid network address in the specified address family", 0);
	if(ip_bin == -1)
		raler("inet_pton", 1);

	if(sendto(sockfd_serveur, sent_request, strlen(sent_request), 0, (struct sockaddr *) &dest, addrlen) == -1)
		raler("sendto", 1);

	if((nb_octets = recvfrom(sockfd_client, buf, 1024, 0, (struct sockaddr *) &dest, &addrlen)) == -1)
		raler("recvfrom", 1);

	printf("msg recu : %s", buf);

	if((close(sockfd_serveur)) == -1)
		raler("close", 1);

	return 0;
}