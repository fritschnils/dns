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


struct serveur{
	char nom[100];
	char ip[40];
	int port;
};
*/

int main(int argc, char const *argv[])
{
	(void)argc;
	(void)argv;

/**************************************************************************/
/* Initialisation des variables et chargement des fichiers                */
/**************************************************************************/
	struct serveur racine[2];
	struct serveur tmp_server[2];
	//struct serveur sousdomaine[NB_SOUS_DOMAINES]; // stocker les serveurs sous_domaine

	int nb_sites = 0;
	int id_transac = 1;
	int sockfd;
    //long int temps_ecoule;

    char horodatage[11];
    char requete_retour[BUFFSIZE];

    struct requete *req_tab = NULL;

	struct timeval start, end;

	struct sockaddr_in6 my_addr, address;


	// INITIALISATION 
	nb_sites = init_client(&req_tab); 

	//Adresses et ports des 2 serveurs racines
	servers_from_file("./lists/infoclient", racine, 2, 1);
	
	//Met à zero le nom des 2 racines, car on ne leur en associe pas
	memset(racine[0].nom, '\0', 100);
	memset(racine[1].nom, '\0', 100);
    

	


/**************************************************************************/
/* Initialisation des variables et chargement des fichiers                */
/**************************************************************************/
    for(int i = 0; i < nb_sites; i++){

    	//ECHANGE AVEC DOMAINE_RESOLVER DEBUT ----------------------------------
    	memset(requete_retour, '\0', BUFFSIZE);
    	memset(horodatage, '\0', 11);
    	gettimeofday(&start, NULL);// NE PAS TARDER A ENVOYER REQUETE 
		timeval_to_str(start, horodatage);

		if(i%2 == 0) //POUR L'INSTANT on choisit 1 sur 2 au lieu de TOURNIQUET
			sockfd = init_socket(&address, racine[0].port, racine[0].ip, 0); //prépare la socket
		if(i%2 == 1)
			sockfd = init_socket(&address, racine[1].port, racine[1].ip, 0); //prépare la socket
		client_request_maker(req_tab[i].req, id_transac, horodatage, req_tab[i].nom); //crée requete send

		snd(sockfd, req_tab[i].req, &address); //envoi requete

		sockfd = init_socket(&my_addr, CLIENT_PORT, CLIENT_ADDR, 1);//crée socket recv

		rcv(sockfd, requete_retour); //recoit
		printf("requete retour : %s\n", requete_retour);

		gettimeofday(&end, NULL);

		//temps_ecoule = (end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec);
		//printf("temps écoulé : %ld\n", temps_ecoule);
		//QUE FAIRE AVEC LE TEMPS ?? IL VA ETRE ECRASE BIENTOT

		reponse_extract_serveur(requete_retour, tmp_server, 0); 
		//printf("EXTRACT SERVER 1 : \nnom : %s\nip : %s\nport : %d\n", tmp_server[0].nom, tmp_server[0].ip, tmp_server[0].port);
		//printf("EXTRACT SERVER 2 : \nnom : %s\nip : %s\nport : %d\n", tmp_server[1].nom, tmp_server[1].ip, tmp_server[1].port);
		
		id_transac++;
		// ECHANGE AVEC DOMAINE_RESOLVER FIN -----------------------------------
		//ECHANGE AVEC SOUS_DOMAINE_RESOLVER DEBUT -----------------------------
		

		memset(requete_retour, '\0', BUFFSIZE);
    	memset(horodatage, '\0', 11);
    	gettimeofday(&start, NULL);// NE PAS TARDER A ENVOYER REQUETE 
		timeval_to_str(start, horodatage);

		if(i%2 == 0) //POUR L'INSTANT on choisit 1 sur 2 au lieu de TOURNIQUET
			sockfd = init_socket(&address, tmp_server[0].port, tmp_server[0].ip, 0); //prépare la socket
		if(i%2 == 1)
			sockfd = init_socket(&address, tmp_server[1].port, tmp_server[1].ip, 0); //prépare la socket
		client_request_maker(req_tab[i].req, id_transac, horodatage, req_tab[i].nom); //crée requete send

		snd(sockfd, req_tab[i].req, &address); //envoi requete

		sockfd = init_socket(&my_addr, CLIENT_PORT, CLIENT_ADDR, 1);//crée socket recv

		rcv(sockfd, requete_retour); //recoit
		printf("requete retour : %s\n", requete_retour);

		gettimeofday(&end, NULL);

		//temps_ecoule = (end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec);
		

		reponse_extract_serveur(requete_retour, tmp_server, 0); 

		id_transac++;
		//ECHANGE AVEC SOUS_DOMAINE_RESOLVER FIN -------------------------------
		//ECHANGE AVEC MACHINE_RESOLVER DEBUT -----------------------------
		

		memset(requete_retour, '\0', BUFFSIZE);
    	memset(horodatage, '\0', 11);
    	gettimeofday(&start, NULL);// NE PAS TARDER A ENVOYER REQUETE 
		timeval_to_str(start, horodatage);

		if(i%2 == 0) //POUR L'INSTANT on choisit 1 sur 2 au lieu de TOURNIQUET
			sockfd = init_socket(&address, tmp_server[0].port, tmp_server[0].ip, 0); //prépare la socket
		if(i%2 == 1)
			sockfd = init_socket(&address, tmp_server[1].port, tmp_server[1].ip, 0); //prépare la socket
		client_request_maker(req_tab[i].req, id_transac, horodatage, req_tab[i].nom); //crée requete send

		snd(sockfd, req_tab[i].req, &address); //envoi requete

		sockfd = init_socket(&my_addr, CLIENT_PORT, CLIENT_ADDR, 1);//crée socket recv

		rcv(sockfd, requete_retour); //recoit
		printf("requete retour : %s\n", requete_retour);
		gettimeofday(&end, NULL);

		//temps_ecoule = (end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec);


		reponse_extract_serveur(requete_retour, tmp_server, 1); 

		id_transac++;
		//ECHANGE AVEC MACHINE_RESOLVER FIN -------------------------------

		printf("A résoudre : %s\rRésolution : %s\n", req_tab[i].nom, tmp_server[0].ip);
		printf("\n");

	}






	//FERMETURE
	//sockfd = init_socket(&address, racine[0].port, racine[0].ip, 0); //prépare la socket
	//snd(sockfd, "!", &address); //ferme racine
	//sockfd = init_socket(&address, racine[1].port, racine[1].ip, 0); //prépare la socket
	//snd(sockfd, "!", &address); //ferme racine
	
	free(req_tab);
    exit(EXIT_SUCCESS);

	return 0;
}