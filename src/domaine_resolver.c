#ifndef FCT_SERVEUR_H
#include "../include/fct_serveur.h"
#endif

/*
NB_RACINES 2
NB_DOMAINES 4
NB_SOUS_DOMAINES 8
NB_MACHINES 16
*/
 
int main(int argc, char const *argv[])
{
	(void)argc;
	(void)argv;

	struct serveur racine[NB_RACINES]; // pour lui
	for(int i = 0; i < NB_RACINES; i++)
		memset(racine[i].nom, '\0', 100);
	struct serveur sousdomaine[NB_DOMAINES]; // pour résoudre

	servers_from_file("./lists/infoclient", racine, NB_RACINES, 1); // pour lui
	printf("\n");
	for(int i = 0; i < NB_RACINES; i++){
		printf("racine%d : ip = %s port = %d nom = %s\n", i, racine[i].ip, racine[i].port, racine[i].nom);
	}

	servers_from_file("./lists/inforacine", sousdomaine, NB_DOMAINES, 0);
	printf("\n");
	for(int i = 0; i < NB_DOMAINES; i++){
		printf("sousdomaine%d : ip = %s port = %d nom = %s\n", i, sousdomaine[i].ip, sousdomaine[i].port, sousdomaine[i].nom);
	}
	
	
	for(int i = 0; i < NB_RACINES; i++){
				switch(fork()){
					case -1 :
						raler("fork", 1);
						break;
					case 0 : 
						request_process(racine[i].port, racine[i].ip);
						break;
				}
	}
	
	n_wait(NB_RACINES);
	return 0;
}