#ifndef FCT_SERVEUR_H
#include "../include/fct_serveur.h"
#endif

/*
NB_RACINES 2
NB_DOMAINES 4
NB_SOUS_DOMAINES 8
NB_MACHINES 16
*/

struct requete {
	char nom[100];
	char *ip;
	int port;
};

void requete_create(struct requete *req, char *dom, char *ip, int port)
{
	int i;
	(void) i;
}

void sitelist_from_file(char *filename, struct requete *req_tab)
{
	char buf[100];
	int i = 0;
	char* test;

	FILE* fichier = NULL;
	if((fichier = fopen(filename, "r")) == NULL)
		raler("fopen", 1);

	while(i+1){
		memset(buf, '\0', 100);
		test = fgets(buf, 99, fichier);
		if(test == NULL)
			break;
		printf("%d\n",i);
		strncpy(req_tab[i].nom, buf, 99);
		i++;
	}

	if(fclose(fichier) != 0)
		raler("fclose", 1);
}

int main(int argc, char const *argv[])
{
	(void)argc;
	(void)argv;
	struct serveur racine[2]; 
	struct serveur domaine[NB_DOMAINES];
	struct serveur sousdomaine[NB_SOUS_DOMAINES];

	struct requete req_tab[17];

	sitelist_from_file("./lists/sites_a_resoudre", req_tab);

	for(int i = 0; i < 17; i++){
		printf("site[%d] : %s\n", i, req_tab[i].nom);
	} 







	



	servers_from_file("./lists/infoclient", racine, 2, 1);


	
	for(int i = 0; i < 2; i++){
		printf("racine%d :\nip = %s\nport = %d\nnom : %s\n", i, racine[i].ip, racine[i].port, racine[i].nom);
	}
	

	
	
	int sockfd;
	char *sent_request = "salut";

	struct sockaddr_in6 my_addr, address;

	for(int i = 0; i < 2; i++){
		sockfd = init_socket(&address, racine[i].port, racine[i].ip, 0);

		snd(sockfd, sent_request, &address);

		sockfd = init_socket(&my_addr, CLIENT_PORT, CLIENT_ADDR, 1);
		rcv(sockfd);
	}


	return 0;
}