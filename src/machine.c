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
	char *sent_request = "machine, recu";
	
	socklen_t addrlen = sizeof(struct sockaddr_in6);
	struct sockaddr_in6 my_addr, client_addr;

	// Initialisation - Reception - Fermeture ---------------------------------
	sockfd = init_socket(&my_addr, MACHINE_PORT, MACHINE_ADDR, addrlen, 1);	
	rcv(sockfd);


	// Initialisation - Envoi - Fermeture ---------------------------------
	sockfd = init_socket(&client_addr, CLIENT_PORT, CLIENT_ADDR, addrlen, 0);
	snd(sockfd, sent_request, &client_addr);


	return 0;
}