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
	long int port_envoi = 40764, port_reception = 50001;
	char buf[1024];
	char *sent_request = "sous_domaine, recu";
	
	socklen_t addrlen = sizeof(struct sockaddr_in);
	struct sockaddr_in my_addr;

	fd_set ens_read;

	// SOCKET RECEPTION INITIALISATION ----------------------------------------
	sockfd = init_socket_reception(&my_addr, port_reception, addrlen);
	
	// RECEPTION AFFICHAGE FERMETURE ------------------------------------------
	FD_ZERO(&ens_read);	
	FD_SET(sockfd, &ens_read);
	if((select(sockfd+1, &ens_read, NULL, NULL, NULL)) == -1)
		raler("select", 1);

	if(FD_ISSET(sockfd, &ens_read)){
		memset(buf, '\0', 1024);
		if((nb_octets = recvfrom(sockfd, buf, 1024, 0, (struct sockaddr *) &my_addr, &addrlen)) == -1)
			raler("recvfrom", 1);

		if((close(sockfd)) == -1)
			raler("close", 1);

		// SOCKET ENVOI INITIALISATION ----------------------------------------
		sockfd = init_socket_envoi(&my_addr, port_envoi);

		// ENVOI PUIS FERMETURE SOCKET ----------------------------------------
		if(nb_octets > 0){
			if(sendto(sockfd, sent_request, strlen(sent_request), 0, (struct sockaddr *) &my_addr, addrlen) == -1)
				raler("sendto", 1);
		}
		else
			printf("ecoute domaine : echec reception\n");	
		
		if((close(sockfd)) == -1)
			raler("close", 1);
	}

	return 0;
}