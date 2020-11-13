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
	struct serveur racine[NB_RACINES];
	struct serveur sousdomaine[NB_DOMAINES];

	//Met à zero le nom des 2 racines, car on ne leur en associe pas
	memset(racine[0].nom, '\0', 100);
	memset(racine[1].nom, '\0', 100);

	//Adresses et ports des 2 serveurs racines (serveurs lancés dans ce programme)
	servers_from_file("./lists/infoclient", racine, NB_RACINES, 1);

	//Liste des seveurs de domaine (fr et com)
	servers_from_file("./lists/inforacine", sousdomaine, NB_DOMAINES, 0);



/**************************************************************************/
/* Lancement des serveurs. 1 serveur par processus et chacun sa socket    */
/**************************************************************************/
	
	for(int i = 0; i < NB_RACINES; i++){
				switch(fork()){
					case -1 :
						raler("fork", 1);
						break;
					case 0 : 
						request_process(racine[i].port, racine[i].ip, sousdomaine, NB_DOMAINES, TYPE_RACINE);
						exit(EXIT_SUCCESS);
						break;
				}
	}
	n_wait(NB_RACINES);
	return 0;
}