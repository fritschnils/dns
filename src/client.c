#ifndef FCT_SERVEUR_H
#include "../include/fct_serveur.h"
#endif


int main(int argc, char const *argv[])
{
	struct serveur racine[2]; 


	servers_from_file("./lists/infoclient", racine, 2);


	/*
	for(int i = 0; i < 2; i++){
		printf("racine%d :\nip = %s\nport = %d\n", i, racine[i].ip, racine[i].port);
	}
	*/
	
	int sockfd;
	char *sent_request = "salut";

	socklen_t addrlen = sizeof(struct sockaddr_in6); 
	struct sockaddr_in6 my_addr, address;

	for(int i = 0; i < 2; i++){
		sockfd = init_socket(&address, racine[i].port, racine[i].ip, addrlen, 0);

		snd(sockfd, sent_request, &address);

		sockfd = init_socket(&my_addr, CLIENT_PORT, CLIENT_ADDR, addrlen, 1);
		rcv(sockfd);
	}
	
	return 0;
}