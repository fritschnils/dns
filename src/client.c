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
	long int port_reception = 40764;
	char *sent_request = "salut";

	socklen_t addrlen = sizeof(struct sockaddr_in6); //sockaddr_storage
	struct sockaddr_in6 my_addr, address;

	for(int i = 0; i < 3; i++){
		// Initialisation - Envoi - Fermeture ---------------------------------
		if(i == 0)
			sockfd = init_socket(&address, 50000, DOMAINE_ADDR, addrlen, 0);
		else if(i == 1)
			sockfd = init_socket(&address, 50001, SOUSDOMAINE_ADDR, addrlen, 0);
		else
			sockfd = init_socket(&address, 50002, MACHINE_ADDR, addrlen, 0);

		snd(sockfd, sent_request, &address);

		// Initialisation - Reception/Affichage - Fermeture -------------------
		sockfd = init_socket(&my_addr, port_reception, CLIENT_ADDR, addrlen, 1);
		rcv(sockfd);
	}
	return 0;
}