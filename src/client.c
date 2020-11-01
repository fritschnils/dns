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
	long int port_envoi = 50000, port_reception = 40764;
	char *sent_request = "salut";
	char buf[1024];

	socklen_t addrlen = sizeof(struct sockaddr_in);
	struct sockaddr_in my_addr;


	// SOCKET ENVOI INITIALISATION --------------------------------------------
	while(port_envoi < 50003){	

		sockfd = init_socket_envoi(&my_addr, port_envoi);


		// ENVOI PUIS FERMETURE SOCKET --------------------------------------------
		if(sendto(sockfd, sent_request, strlen(sent_request), 0, (struct sockaddr *) &my_addr, addrlen) == -1)
			raler("sendto", 1);

		if((close(sockfd)) == -1)
			raler("close", 1);

		// SOCKET RECEPTION INITIALISATION ----------------------------------------
		sockfd = init_socket_reception(&my_addr, port_reception, addrlen);
		memset(buf, '\0', 1024);
		// RECEPTION AFFICHAGE FERMETURE -- ---------------------------------------
		if((nb_octets = recvfrom(sockfd, buf, 1024, 0, (struct sockaddr *) &my_addr, &addrlen)) == -1)
			raler("recvfrom", 1);

		printf("message recu : %s\n", buf);

		if((close(sockfd)) == -1)
			raler("close", 1);
		port_envoi++;
	}

	return 0;
}