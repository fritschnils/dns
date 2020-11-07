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

	for(int i = 0; i < 9; i++){
		// Initialisation - Envoi - Fermeture ---------------------------------
		switch(i){
			/********************** PING DOMAINE *****************************/
			case 0:
				sockfd = init_socket(&address, DOMAINE1_PORT, DOMAINE1_ADDR, addrlen, 0);
				break;
			case 1:
				sockfd = init_socket(&address, DOMAINE2_PORT, DOMAINE2_ADDR, addrlen, 0);
				break;
			case 2:
				sockfd = init_socket(&address, DOMAINE3_PORT, DOMAINE3_ADDR, addrlen, 0);
				break;

			/*********************** PING SOUS_DOMAINE ***********************/
			case 3:
				sockfd = init_socket(&address, SOUSDOMAINE1_PORT, SOUSDOMAINE1_ADDR, addrlen, 0);
				break;
			case 4:
				sockfd = init_socket(&address, SOUSDOMAINE2_PORT, SOUSDOMAINE2_ADDR, addrlen, 0);
				break;
			case 5:
				sockfd = init_socket(&address, SOUSDOMAINE3_PORT, SOUSDOMAINE3_ADDR, addrlen, 0);
				break;

			/************************* PING MACHINE **************************/
			case 6:
				sockfd = init_socket(&address, MACHINE1_PORT, MACHINE1_ADDR, addrlen, 0);
				break;
			case 7:
				sockfd = init_socket(&address, MACHINE2_PORT, MACHINE2_ADDR, addrlen, 0);
				break;
			case 8:
				sockfd = init_socket(&address, MACHINE3_PORT, MACHINE3_ADDR, addrlen, 0);
				break;
		}
		snd(sockfd, sent_request, &address);

		// Initialisation - Reception/Affichage - Fermeture -------------------
		sockfd = init_socket(&my_addr, CLIENT_PORT, CLIENT_ADDR, addrlen, 1);
		rcv(sockfd);
	}
	return 0;
}