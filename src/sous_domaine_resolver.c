#ifndef FCT_SERVEUR_H
#include "../include/fct_serveur.h"
#endif

int main(int argc, char const *argv[])
{
	(void)argc;
	(void)argv;

/**************************************************************************/
/* Initialisation des variables et chargement des fichiers                */
/**************************************************************************/
	struct serveur sousdomaine[NB_DOMAINES];
	struct serveur sousdomaine_fr[NB_SOUS_DOMAINES/2];
	struct serveur sousdomaine_com[NB_SOUS_DOMAINES/2];

	//Adresses et ports des 4 serveurs domaines (serveurs lancés dans ce programme)
	servers_from_file("./lists/inforacine", sousdomaine, NB_DOMAINES, 0); 

	//Liste des seveurs de sous_domaine (a.fr, b.fr, a.com et b.com en plusieurs versions)
	servers_from_file("./lists/sousdomaine/fr", sousdomaine_fr, NB_SOUS_DOMAINES/2, 0);
	servers_from_file("./lists/sousdomaine/com", sousdomaine_com, NB_SOUS_DOMAINES/2, 0);

	
/**************************************************************************/
/* Lancement des serveurs. 1 serveur par processus et chacun sa socket    */
/**************************************************************************/
	for(int i = 0; i < NB_DOMAINES; i++){
		switch(fork()){
			case -1 :
				raler("fork", 1);
				break;
			case 0 : 
				if(i < 2){
					request_process(sousdomaine[i].port, sousdomaine[i].ip, sousdomaine_fr, NB_SOUS_DOMAINES/2, TYPE_SOUSDOMAINE); //FR
					exit(EXIT_SUCCESS);
				}
				else{
					request_process(sousdomaine[i].port, sousdomaine[i].ip, sousdomaine_com, NB_SOUS_DOMAINES/2, TYPE_SOUSDOMAINE); //COM
					exit(EXIT_SUCCESS);
					break;
					}
		}
	}
	n_wait(NB_DOMAINES);
	return 0;
}