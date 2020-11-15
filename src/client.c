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
	(void) argv;
	(void) argc;

	struct stat attributs1;

	//Début vérifications argument
	if(argc != 2)
		raler("mauvais nombre d'arguments", 0);

	if(stat(argv[1], &attributs1) == -1)
		raler("stat", 1);

	if(attributs1.st_size == 0)
		raler("fichier vide", 0);

	if(S_ISREG(attributs1.st_mode)){
		if(access(argv[1], F_OK) == -1)
			raler("fichier inaccessible", 0);
	}
	else
		raler("fichier non-regulier", 0);
	//Fin vérifications argument

/**************************************************************************/
/* Initialisation des variables et chargement des fichiers                */
/**************************************************************************/
	struct serveur racine[2];
	struct serveur tmp_server[2];
	//struct serveur sousdomaine[NB_SOUS_DOMAINES]; // stocker les serveurs sous_domaine

	int nb_sites = 0;
	int id_transac = 1;
	int sockfd_envoi, sockfd_reception;
	//long int temps_ecoule;

	char horodatage[11];
	char horodatage_result[11];
	char requete_retour[BUFFSIZE];

	struct requete *req_tab = NULL;

	struct timeval total, start, end, result;

	struct sockaddr_in6 my_addr, address;


	// Initialise le tableau de requêtes et stock son nombre
	nb_sites = init_client(&req_tab, argv[1]); 

	//Adresses et ports des 2 serveurs racines
	servers_from_file("./lists/infoclient", racine, 2, 1);
	
	//Met à zero le nom des 2 racines, car on ne leur en associe pas
	memset(racine[0].nom, '\0', 100);
	memset(racine[1].nom, '\0', 100);
    
	gettimeofday(&total, NULL);

	FILE* log = NULL;
	if((log = fopen("log", "w")) == NULL)
		raler("open log", 1);

	fprintf(log, "eddzzefz\n");

/**************************************************************************/
/* Lancement de la résolution des requêtes une par une                    */
/**************************************************************************/
	for(int i = 0; i < nb_sites; i++){

	//DEBUT ECHANGE AVEC DOMAINE_RESOLVER (SERVEURS RACINES) ---------------------------
		memset(requete_retour, '\0', BUFFSIZE);
		memset(horodatage, '\0', 11);
		memset(horodatage_result, '\0', 11);
		gettimeofday(&start, NULL);
		timeval_to_str(start, horodatage);

		//Tourniquet
		if(i%2 == 0)
			sockfd_envoi = init_socket(&address, racine[0].port, racine[0].ip, 0);
		if(i%2 == 1)
			sockfd_envoi = init_socket(&address, racine[1].port, racine[1].ip, 0);

		//Crée requête
		client_request_maker(req_tab[i].req, id_transac, horodatage, req_tab[i].nom); 

		//Envoi requête à racine
		printf("envoie a racine :        %s\n", req_tab[i].req);
		snd(sockfd_envoi, req_tab[i].req, &address);

		//Crée socket pour recevoir
		sockfd_reception = init_socket(&my_addr, CLIENT_PORT, CLIENT_ADDR, 1);

		//Reçoit réponse de racine
		rcv(sockfd_reception, requete_retour);     	
		gettimeofday(&end, NULL);
		result.tv_sec = end.tv_sec - start.tv_sec;
		result.tv_usec = end.tv_usec - start.tv_usec;

		timeval_to_str(result, horodatage_result);

		printf("requete retour :         %s temps : %s\n", requete_retour, horodatage_result);

		//Extrait les infos reçues (adresses des serveurs de domaine)
		if(reponse_extract_serveur(requete_retour, tmp_server, 0) == -1){
			printf("Site inexistant\n");	
			fprintf(log, "Site inexistant\n");	
			continue;
		}
		
		//Transaction suivante
		id_transac++;

	//FIN ECHANGE AVEC DOMAINE_RESOLVER (SERVEURS RACINES) -----------------------------
		


	//DEBUT ECHANGE AVEC SOUS_DOMAINE_RESOLVER (SERVEURS DOMAINES) ---------------------
		memset(requete_retour, '\0', BUFFSIZE);
		memset(horodatage, '\0', 11);
		gettimeofday(&start, NULL);
		timeval_to_str(start, horodatage);

		if(i%2 == 0) //Tourniquet
			sockfd_envoi = init_socket(&address, tmp_server[0].port, tmp_server[0].ip, 0);
		if(i%2 == 1)
			sockfd_envoi = init_socket(&address, tmp_server[1].port, tmp_server[1].ip, 0);

		//Crée requête
		client_request_maker(req_tab[i].req, id_transac, horodatage, req_tab[i].nom);

		//Envoi requête à domaine
		printf("envoie a domaine :       %s\n", req_tab[i].req);
		snd(sockfd_envoi, req_tab[i].req, &address);
		
		//Crée socket pour recevoir
		sockfd_reception = init_socket(&my_addr, CLIENT_PORT, CLIENT_ADDR, 1);

		//Reçoit réponse de domaine
		rcv(sockfd_reception, requete_retour); 
		gettimeofday(&end, NULL);
		result.tv_sec = end.tv_sec - start.tv_sec;
		result.tv_usec = end.tv_usec - start.tv_usec;

		timeval_to_str(result, horodatage_result);

		printf("requete retour :         %s temps : %s\n", requete_retour, horodatage_result);
		//Extrait les infos reçues (adresses des serveurs de sous_domaine)
		if(reponse_extract_serveur(requete_retour, tmp_server, 0) == -1){
			printf("Site inexistant\n");
			fprintf(log, "Site inexistant\n");
			continue;
		}

		//Transaction suivante
		id_transac++;

	//FIN ECHANGE AVEC SOUS_DOMAINE_RESOLVER (SERVEURS DOMAINES) -----------------------
	
	//DEBUT ECHANGE AVEC MACHINE_RESOLVER (SERVEURS SOUS-DOMAINE) ----------------------
		memset(requete_retour, '\0', BUFFSIZE);
		memset(horodatage, '\0', 11);
		gettimeofday(&start, NULL);
		timeval_to_str(start, horodatage);

		if(i%2 == 0) //Tourniquet
			sockfd_envoi = init_socket(&address, tmp_server[0].port, tmp_server[0].ip, 0);
		if(i%2 == 1)
			sockfd_envoi = init_socket(&address, tmp_server[1].port, tmp_server[1].ip, 0);
		
		//Crée requête
		client_request_maker(req_tab[i].req, id_transac, horodatage, req_tab[i].nom);

		//Envoi requête à sous-domaine
		printf("envoie a sous_domaine :  %s\n", req_tab[i].req);
		snd(sockfd_envoi, req_tab[i].req, &address); //envoi requete

		//Crée socket pour recevoir
		sockfd_reception = init_socket(&my_addr, CLIENT_PORT, CLIENT_ADDR, 1);

		//Reçoit réponse de sous_domaine
		rcv(sockfd_reception, requete_retour);
		gettimeofday(&end, NULL);
		result.tv_sec = end.tv_sec - start.tv_sec;
		result.tv_usec = end.tv_usec - start.tv_usec;

		timeval_to_str(result, horodatage_result);

		printf("requete retour :         %s temps : %s\n", requete_retour, horodatage_result);
		//Extrait les infos reçues (adresses des serveurs de machines)
		if(reponse_extract_serveur(requete_retour, tmp_server, 1) == -1){
			printf("Site inexistant\n");
			fprintf(log, "Site inexistant\n");
			continue;
		}

		//Transaction suivante
		id_transac++;

	//FIN ECHANGE AVEC MACHINE_RESOLVER (SERVEURS SOUS-DOMAINE) ------------------------
		fprintf(log, "%s\n", tmp_server[0].ip);
		printf("\rRésolution : %s\n", tmp_server[0].ip);
		printf("\n");
	}


	gettimeofday(&end, NULL);
	printf("TEMPS : %ldms\n", ((end.tv_sec * 1000000 + end.tv_usec) - (total.tv_sec * 1000000 + total.tv_usec))/1000 );
	
	if(fclose(log) != 0)
		raler("close log", 1);

	free(req_tab);
	exit(EXIT_SUCCESS);

	return 0;
}