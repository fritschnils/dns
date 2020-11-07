#ifndef FCT_SERVEUR_H
#include "../include/fct_serveur.h"
#endif

int main(int argc, char const *argv[])
{
	/*
	int sockfd;	
	struct sockaddr_in6 my_addr, client_addr;
	*/
	int reason;

	for(int i=0;i<3;i++){
		switch(i){
			case 0:
				switch(fork()){
					case -1 :
						raler("fork", 1);
						break;
					case 0 : 
						domaine_fils1();
				}
				break;

			case 1:
				switch(fork()){
					case -1 :
							raler("fork", 1);
						break;
					case 0 : 
						domaine_fils2();
				}
				break;

			case 2:
				switch(fork()){
					case -1 :
						raler("fork", 1);
							break;
					case 0 : 
						domaine_fils3();
				}
				break;
		}
	}
	/*
	// Initialisation - Reception - Fermeture ---------------------------------
	sockfd = init_socket(&my_addr, port_reception, DOMAINE_ADDR, addrlen, 1);	
	rcv(sockfd);


	// Initialisation - Envoi - Fermeture ---------------------------------
	sockfd = init_socket(&client_addr, CLIENT_PORT, CLIENT_ADDR, addrlen, 0);
	snd(sockfd, sent_request, &client_addr);
	*/

	for(int j=0; j<3;j++){
		if(wait(&reason) == -1)
			raler("wait", 1);
		if(!WIFEXITED(reason) || WEXITSTATUS(reason) == EXIT_FAILS)
			raler("child terminated abnormally", 0);
	}
	exit(EXIT_SUCCESS);
	return 0;
}