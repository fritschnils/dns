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
	char *sent_request = "salut";

	socklen_t addrlen = sizeof(struct sockaddr_in6); //sockaddr_storage
	struct sockaddr_in6 my_addr, address;

	for(int i = 0; i < 5; i++){
		// Initialisation - Envoi - Fermeture ---------------------------------
		switch(i){
			case 0:
				sockfd = init_socket(&address, DOMAINE1_PORT, DOMAINE1_ADDR, addrlen, 0);
				break;
			case 1:
				sockfd = init_socket(&address, DOMAINE2_PORT, DOMAINE2_ADDR, addrlen, 0);
				break;
			case 2:
				sockfd = init_socket(&address, DOMAINE3_PORT, DOMAINE3_ADDR, addrlen, 0);
				break;
			case 3:
				sockfd = init_socket(&address, SOUSDOMAINE_PORT, SOUSDOMAINE_ADDR, addrlen, 0);
				break;
			case 4:
				sockfd = init_socket(&address, MACHINE_PORT, MACHINE_ADDR, addrlen, 0);
				break;
		}
		snd(sockfd, sent_request, &address);

		// Initialisation - Reception/Affichage - Fermeture -------------------
		sockfd = init_socket(&my_addr, CLIENT_PORT, CLIENT_ADDR, addrlen, 1);
		rcv(sockfd);
	}
	return 0;
}