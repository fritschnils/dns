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

void timeval_to_str(struct timeval start, char *str)
{
    int nombre = start.tv_sec + start.tv_usec;
    sprintf(str, "%d%c", nombre, '\0');
}

void client_request_maker(char *storage, int id_transac, char *horodatage, char *nom)
{
	sprintf(storage, "%d|%s|%s%c", id_transac, horodatage, nom, '\0');
}


int main(int argc, char const *argv[])
{
	(void)argc;
	(void)argv;
	int id_transac = 1;
	struct serveur racine[2];  
	struct serveur domaine[NB_DOMAINES];
	struct serveur sousdomaine[NB_SOUS_DOMAINES];

	struct requete *req_tab = NULL;
	int nb_sites = 0;

	// INITIALISATION 
	nb_sites = init_client(&req_tab); // retourne le nb de requetes et les alloue

	/*
	for(int i = 0; i < nb_sites; i++){
		printf("site[%d] : %s\n", i, req_tab[i].nom);
	} 
	*/

	servers_from_file("./lists/infoclient", racine, 2, 1);
	memset(racine[0].nom, '\0', 40);
	memset(racine[1].nom, '\0', 40);


	/*
	for(int i = 0; i < 2; i++){
		printf("racine%d :\nip = %s\nport = %d\nnom : %s\n", i, racine[i].ip, racine[i].port, racine[i].nom);
	}
	*/
	// FIN INITIALISATION
	
	struct timeval start, end, result;
	//gettimeofday(&start, NULL);

    char str[10];
    char receive[BUFFSIZE];
    memset(receive, '\0', BUFFSIZE);
	//timeval_to_str(start, str);
    //printf("%s\n", str);
	int sockfd;
	char *sent_request = "salut";
	struct sockaddr_in6 my_addr, address;


    //for(int i = 0; i < nb_sites; i++){
		int i = 0;
    	memset(str, '\0', 10);
    	gettimeofday(&start, NULL);
		timeval_to_str(start, str);// NE PAS TARDER A ENVOYER REQUETE 

		sockfd = init_socket(&address, racine[i].port, racine[i].ip, 0); //prépare la socket

		client_request_maker(req_tab[i].req, id_transac, str, req_tab[i].nom); //crée requete send

		snd(sockfd, req_tab[i].req, &address); //envoi requete

		sockfd = init_socket(&my_addr, CLIENT_PORT, CLIENT_ADDR, 1);//crée requete recv

		rcv(sockfd, receive); //recoit

		//printf("%s\n", req_tab[i].req);
		id_transac++;
	//}



    exit(EXIT_SUCCESS);





/*
	int sockfd;
	char *sent_request = "salut";

	struct sockaddr_in6 my_addr, address;

	for(int i = 0; i < 2; i++){
		sockfd = init_socket(&address, racine[i].port, racine[i].ip, 0);

		snd(sockfd, req_tab[i].nom, &address);

		sockfd = init_socket(&my_addr, CLIENT_PORT, CLIENT_ADDR, 1);
		rcv(sockfd);
	}

	*/
	free(req_tab);
	return 0;
}