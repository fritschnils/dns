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

int init_socket_reception(struct sockaddr_in *address, long int port, int addrlen)
{	
	int sockfd = -1;
	if((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
		raler("socket", 1);

	address->sin_family = AF_INET;
	address->sin_port = htons(port);
	address->sin_addr.s_addr = htonl(INADDR_ANY);

	if((bind(sockfd, (struct sockaddr *) address, addrlen)) == -1)
		raler("bind", 1);

	return sockfd;
}

int main(int argc, char const *argv[])
{
	int sockfd, aff_port, ip_bin;
	ssize_t nb_octets;
	long int port_envoi = 40764, port_reception = 50764;
	char buf[1024], addr_to_print[INET6_ADDRSTRLEN];
	char *sent_request = "bien recu";
	
	socklen_t addrlen = sizeof(struct sockaddr_in);
	struct sockaddr_in my_addr;

	fd_set ens_read;

	// SOCKET RECEPTION INITIALISATION ----------------------------------------
	/*if((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
		raler("socket", 1);

	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(port_reception);
	my_addr.sin_addr.s_addr = htonl(INADDR_ANY);*/
	sockfd = init_socket_reception(&my_addr, port_reception, addrlen);
	
	/*if((bind(sockfd, (struct sockaddr *) &my_addr, addrlen)) == -1)
		raler("bind", 1);*/
	memset(buf, '\0', 1024);

	// RECEPTION AFFICHAGE FERMETURE ------------------------------------------
	FD_ZERO(&ens_read);	
	FD_SET(sockfd, &ens_read);
	if((select(sockfd+1, &ens_read, NULL, NULL, NULL)) == -1)
		raler("select", 1);

	if(FD_ISSET(sockfd, &ens_read)){
		if((nb_octets = recvfrom(sockfd, buf, 1024, 0, (struct sockaddr *) &my_addr, &addrlen)) == -1)
			raler("recvfrom", 1);

		if(inet_ntop(AF_INET, &my_addr.sin_addr.s_addr,addr_to_print, INET6_ADDRSTRLEN) == NULL)
			raler("inet_ntop", 1);
		aff_port = ntohs(my_addr.sin_port);
		printf("message reçu : %s\noctets reçus : %ld\nadresse expéditeur : %s\nport expéditeur : %d\n", buf, nb_octets, addr_to_print, aff_port);

		if((close(sockfd)) == -1)
			raler("close", 1);

		// SOCKET ENVOI INITIALISATION ----------------------------------------
		if((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
			raler("socket", 1);

		my_addr.sin_family = AF_INET;
		my_addr.sin_port = htons(port_envoi);
		ip_bin = inet_pton(AF_INET, "127.0.0.1", &my_addr.sin_addr);
		if(ip_bin == 0)
			raler("inet_pton : src does not contain a character string representing a valid network address in the specified address family", 0);
		if(ip_bin == -1)
			raler("inet_pton", 1);

		// ENVOI PUIS FERMETURE SOCKET ----------------------------------------
		if(sendto(sockfd, sent_request, strlen(sent_request), 0, (struct sockaddr *) &my_addr, addrlen) == -1)
			raler("sendto", 1);

		if((close(sockfd)) == -1)
			raler("close", 1);
	}

	return 0;
}