#ifndef FCT_SERVEUR_H
#include "../include/fct_serveur.h"
#endif

/*
NB_RACINES 2
NB_DOMAINES 4
NB_SOUS_DOMAINES 8
NB_MACHINES 16


struct requete {
	char req[150];
	char nom[100];
	char ip[40];
	int port;
};
*/

int main(int argc, char const *argv[])
{
	(void)argc;
	(void)argv;

	struct serveur racine[2];  // stocker les serveurs racines
	struct serveur domaine[NB_DOMAINES]; // stocker les serveurs domaines
	struct serveur sousdomaine[NB_SOUS_DOMAINES]; // stocker les serveurs sous_domaine

	int nb_sites = 0;
	int id_transac = 1;
	int sockfd;
    long int temps_ecoule;

    char horodatage[10];
    char requete_retour[BUFFSIZE];

    struct requete *req_tab = NULL;

	struct timeval start, end, result;

	struct sockaddr_in6 my_addr, address;


	// INITIALISATION 
	nb_sites = init_client(&req_tab); // retourne le nb de requetes et les alloue

	servers_from_file("./lists/infoclient", racine, 2, 1);

	memset(racine[0].nom, '\0', 40);
	memset(racine[1].nom, '\0', 40);
    memset(requete_retour, '\0', BUFFSIZE);

	// FIN INITIALISATION
	



    for(int i = 0; i < nb_sites; i++){

    	memset(horodatage, '\0', 10);
    	gettimeofday(&start, NULL);// NE PAS TARDER A ENVOYER REQUETE 
		timeval_to_str(start, horodatage);
		if(i%2 == 0)
			sockfd = init_socket(&address, racine[0].port, racine[0].ip, 0); //prépare la socket
		if(i%2 == 1)
			sockfd = init_socket(&address, racine[1].port, racine[1].ip, 0); //prépare la socket
		client_request_maker(req_tab[i].req, id_transac, horodatage, req_tab[i].nom); //crée requete send

		snd(sockfd, req_tab[i].req, &address); //envoi requete

		sockfd = init_socket(&my_addr, CLIENT_PORT, CLIENT_ADDR, 1);//crée socket recv

		rcv(sockfd, requete_retour); //recoit
		
		gettimeofday(&end, NULL);
		temps_ecoule = (end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec);
		printf("temps écoulé : %ldms\n", temps_ecoule/1000);

		id_transac++;
	}




	//FERMETURE
	sockfd = init_socket(&address, racine[0].port, racine[0].ip, 0); //prépare la socket
	snd(sockfd, "!", &address); //ferme racine
	sockfd = init_socket(&address, racine[1].port, racine[1].ip, 0); //prépare la socket
	snd(sockfd, "!", &address); //ferme racine
	
	free(req_tab);
    exit(EXIT_SUCCESS);

	return 0;
}