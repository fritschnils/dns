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

	struct serveur sousdomaine[NB_DOMAINES];//pour lui

	struct serveur sousdomaine_fr[NB_SOUS_DOMAINES/2]; //pour résoudre
	struct serveur sousdomaine_com[NB_SOUS_DOMAINES/2]; //pour résoudre
	
	struct timeval start, end;
	gettimeofday(&start, NULL);


	servers_from_file("./lists/inforacine", sousdomaine, NB_DOMAINES, 0); //pour lui
	// affichage
	printf("\n");
	for(int i = 0; i < NB_DOMAINES; i++){
	printf("sousdomaine%d : ip = %s port = %d nom = %s \n", i, sousdomaine[i].ip, sousdomaine[i].port, sousdomaine[i].nom);
	}

	servers_from_file("./lists/sousdomaine/fr", sousdomaine_fr, NB_SOUS_DOMAINES/2, 0);
	/* affichage*/
	printf("\n");
	for(int i = 0; i < NB_SOUS_DOMAINES/2; i++){
		printf("sousdomaine_fr%d : ip = %s port = %d nom = %s \n", i, sousdomaine_fr[i].ip, sousdomaine_fr[i].port, sousdomaine_fr[i].nom);
	}
	
	servers_from_file("./lists/sousdomaine/com", sousdomaine_com, NB_SOUS_DOMAINES/2, 0);
	/* affichage*/
	printf("\n");
	for(int i = 0; i < NB_SOUS_DOMAINES/2; i++){
		printf("sousdomaine_com%d : ip = %s port = %d nom = %s \n", i, sousdomaine_com[i].ip, sousdomaine_com[i].port, sousdomaine_com[i].nom);
	
	}

  	gettimeofday(&end, NULL);
  	printf("Time taken  : %ld micro seconds\n",
    ((end.tv_sec * 1000000 + end.tv_usec) -
    (start.tv_sec * 1000000 + start.tv_usec)));
	
	// Création processus
	for(int i = 0; i < NB_DOMAINES; i++){
		switch(fork()){
			case -1 :
				raler("fork", 1);
				break;
			case 0 : 
				if(i < 2)
					//AFFECTER TABLEAU DE SERVEURS POUR RESOUDRE
					request_process(sousdomaine[i].port, sousdomaine[i].ip); //FR
				else
					//AFFECTER TABLEAU DE SERVEURS POUR RESOUDRE
					request_process(sousdomaine[i].port, sousdomaine[i].ip); //COM
				break;
		}
	}
	
	n_wait(NB_DOMAINES);
	
	return 0;
}