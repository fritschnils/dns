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

void reponse_extract_serveur(char *reponse, struct serveur *storage, int ismachine)
{
	int repere = 4, i = 0, j = 0;
	char port[6];

	while(repere){ //avance jusqu'au premier triplet
		if(reponse[i] == '|')
			repere--;
		i++;
	}

	for(int n = 0; n < 2; n++){
		while(reponse[i] != ','){//recupere le nom du serveur
			storage[n].nom[j] = reponse[i];
			i++;
			j++;
		}
		storage[n].nom[j] = '\0';	
		i++;
		j = 0;
	
		while(reponse[i] != ','){//recupere l'ip du serveur
			storage[n].ip[j] = reponse[i];
			i++;
			j++;
		}
		storage[n].ip[j] = '\0';	
		i++;
		j = 0;
		//printf("nom : %s\nip :  %s\n", storage[n].nom, storage[n].ip);
		while( (reponse[i] != '|' && n == 0 && !ismachine) || (reponse[i] != '\0' && n == 1) || (reponse[i] != '\0' && ismachine)){//recupere le port du serveur
			port[j] = reponse[i];
			i++;
			j++;
		}
		port[j] = '\0';
		storage[n].port = atoi(port);
		if(ismachine)
			return;
		i++;
		j = 0;
	}

}


int main(int argc, char const *argv[])
{
	(void)argc;
	(void)argv;

	struct serveur racine[2];  // stocker les serveurs racines
	struct serveur tmp_server[2]; // stocker les serveurs temporaires
	//struct serveur sousdomaine[NB_SOUS_DOMAINES]; // stocker les serveurs sous_domaine

	int nb_sites = 0;
	int id_transac = 1;
	int sockfd;
    long int temps_ecoule;

    char horodatage[11];
    char requete_retour[BUFFSIZE];

    struct requete *req_tab = NULL;

	struct timeval start, end, result;

	struct sockaddr_in6 my_addr, address;


	// INITIALISATION 

	nb_sites = init_client(&req_tab); // retourne le nb de requetes et les alloue

	servers_from_file("./lists/infoclient", racine, 2, 1);

	memset(racine[0].nom, '\0', 40);
	memset(racine[1].nom, '\0', 40);
    

	// FIN INITIALISATION
	



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
		gettimeofday(&end, NULL);

		temps_ecoule = (end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec);
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
		gettimeofday(&end, NULL);

		temps_ecoule = (end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec);
		

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
		gettimeofday(&end, NULL);

		temps_ecoule = (end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec);


		reponse_extract_serveur(requete_retour, tmp_server, 1); 

		id_transac++;
		//ECHANGE AVEC MACHINE_RESOLVER FIN -------------------------------

		printf("requete : %s\rresolution : %s\n", req_tab[i].nom, tmp_server[0].ip);
		printf("\n");

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