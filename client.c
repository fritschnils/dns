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
	int sockfd, ip_bin;
	ssize_t nb_octets;
	long int port_envoi = 50764, port_reception = 40764;
	char *sent_request = "salut";
	char buf[1024];

	socklen_t addrlen;
	struct sockaddr_in my_addr;


	// SOCKET ENVOI INITIALISATION --------------------------------------------
	if((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
		raler("socket", 1);

	addrlen = sizeof(struct sockaddr_in);
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(port_envoi);
	ip_bin = inet_pton(AF_INET, "127.0.0.1", &my_addr.sin_addr);
	if(ip_bin == 0)
		raler("inet_pton : src does not contain a character string representing a valid network address in the specified address family", 0);
	if(ip_bin == -1)
		raler("inet_pton", 1);

	// ENVOI PUIS FERMETURE SOCKET --------------------------------------------
	if(sendto(sockfd, sent_request, strlen(sent_request), 0, (struct sockaddr *) &my_addr, addrlen) == -1)
		raler("sendto", 1);

	if((close(sockfd)) == -1)
		raler("close", 1);

	// SOCKET RECEPTION INITIALISATION ----------------------------------------
	if((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
		raler("socket", 1);

	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(port_reception);
	my_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	memset(buf, '\0', 1024);
	if((bind(sockfd, (struct sockaddr *) &my_addr, addrlen)) == -1)
		raler("bind2", 1);

	// RECEPTION AFFICHAGE FERMETURE -- ---------------------------------------
	if((nb_octets = recvfrom(sockfd, buf, 1024, 0, (struct sockaddr *) &my_addr, &addrlen)) == -1)
		raler("recvfrom", 1);

	printf("message recu : %s\n", buf);

	if((close(sockfd)) == -1)
		raler("close", 1);

	return 0;
}