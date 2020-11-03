#include <sys/types.h>	
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <limits.h>
#include <string.h>
#ifndef FCT_SERVEUR_H
#include "../include/fct_serveur.h"
#endif

int main(int argc, char const *argv[])
{
	int sockfd;
	ssize_t nb_octets;
	long int port_reception = 40764;
	char *sent_request = "salut";
	char buf[BUFFSIZE];

	socklen_t addrlen = sizeof(struct sockaddr_in);
	struct sockaddr_in my_addr, address;

	for(int i = 0; i < 3; i++){
		// Initialisation - Envoi - Fermeture ---------------------------------
		if(i == 0)
			sockfd = init_socket(&address, 50000, DOMAINE_ADDR, addrlen, 0);
		else if(i == 1)
			sockfd = init_socket(&address, 50001, SOUSDOMAINE_ADDR, addrlen, 0);
		else
			sockfd = init_socket(&address, 50002, MACHINE_ADDR, addrlen, 0);

		if(sendto(sockfd, sent_request, strlen(sent_request), 0, (struct sockaddr *) &address, addrlen) == -1)
			raler("sendto", 1);

		if((close(sockfd)) == -1)
			raler("close", 1);

		// Initialisation - Reception/Affichage - Fermeture -------------------
		sockfd = init_socket(&my_addr, port_reception, CLIENT_ADDR, addrlen, 1);

		memset(buf, '\0', BUFFSIZE);

		if((nb_octets = recvfrom(sockfd, buf, BUFFSIZE, 0, (struct sockaddr *) &my_addr, &addrlen)) == -1)
			raler("recvfrom", 1);

		printf("message recu : %s\n", buf);

		if((close(sockfd)) == -1)
			raler("close", 1);
	}

	return 0;
}