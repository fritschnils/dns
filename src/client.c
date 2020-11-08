#ifndef FCT_SERVEUR_H
#include "../include/fct_serveur.h"
#endif


int main(int argc, char const *argv[])
{
	//char buf[40];
	//struct serveur *racine = malloc(2*sizeof(struct serveur));
	//int test, i = 0;
	struct serveur racine[2]; 
	FILE* fichier = NULL;
	if((fichier = fopen("./lists/infoclient", "r")) == NULL)
		raler("fopen", 1);

	servers_from_file(fichier, racine, 2);
	/*while(i < 2){
		while(test != '\n'){
			memset(buf, '\0', 40);
			test = element_from_file(fichier, buf);
			//printf("chaine récupérée : %s\ndernier char rencontré : %c\n", buf, test);
	
			if(test == '|')
				snprintf(racine[i].ip, 39, buf);
			if(test == '\n')
				racine[i].port = atoi(buf);
		}
		printf("racine%d :\nip = %s\nport = %d\n", i, racine[i].ip, racine[i].port);
		i++;
		test = 0;
	}
	*/

	if(fclose(fichier) != 0)
		raler("fclose", 1);

	for(int i = 0; i < 2; i++){
		printf("racine%d :\nip = %s\nport = %d\n", i, racine[i].ip, racine[i].port);
	}

/*
	
	int sockfd;
	char *sent_request = "salut";

	socklen_t addrlen = sizeof(struct sockaddr_in6); 
	struct sockaddr_in6 my_addr, address;

	for(int i = 0; i < 9; i++){
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
				sockfd = init_socket(&address, SOUSDOMAINE1_PORT, SOUSDOMAINE1_ADDR, addrlen, 0);
				break;
			case 4:
				sockfd = init_socket(&address, SOUSDOMAINE2_PORT, SOUSDOMAINE2_ADDR, addrlen, 0);
				break;
			case 5:
				sockfd = init_socket(&address, SOUSDOMAINE3_PORT, SOUSDOMAINE3_ADDR, addrlen, 0);
				break;

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

		sockfd = init_socket(&my_addr, CLIENT_PORT, CLIENT_ADDR, addrlen, 1);
		rcv(sockfd);
	}
	*/
	return 0;
}