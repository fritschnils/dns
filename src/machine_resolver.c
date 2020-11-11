#ifndef FCT_SERVEUR_H
#include "../include/fct_serveur.h"
#endif

//NB_RACINES 2
//NB_DOMAINES 4
//NB_SOUS_DOMAINES 8
//NB_MACHINES 16

int main(int argc, char const *argv[])
{	
	(void)argc;
	(void)argv;

	struct serveur sousdomaine_fr[NB_SOUS_DOMAINES/2]; //pour lui
	struct serveur sousdomaine_com[NB_SOUS_DOMAINES/2]; //pour lui

	struct serveur machine_a_fr[NB_MACHINES/4]; //pour résoudre
	struct serveur machine_b_fr[NB_MACHINES/4]; //pour résoudre
	struct serveur machine_a_com[NB_MACHINES/4]; //pour résoudre
	struct serveur machine_b_com[NB_MACHINES/4]; //pour résoudre

	




	servers_from_file("./lists/sousdomaine/fr", sousdomaine_fr, NB_SOUS_DOMAINES/2, 0); //pour lui
	printf("\n");
	for(int i = 0; i < NB_SOUS_DOMAINES/2; i++){
		printf("sousdomaine_fr%d : ip = %s port = %d nom = %s \n", i, sousdomaine_fr[i].ip, sousdomaine_fr[i].port, sousdomaine_fr[i].nom);
	}
	
	servers_from_file("./lists/sousdomaine/com", sousdomaine_com, NB_SOUS_DOMAINES/2, 0); //pour lui
	printf("\n");
	for(int i = 0; i < NB_SOUS_DOMAINES/2; i++){
		printf("sousdomaine_com%d : ip = %s port = %d nom = %s \n", i, sousdomaine_com[i].ip, sousdomaine_com[i].port, sousdomaine_com[i].nom);
	}


	
	servers_from_file("./lists/machine/a.fr", machine_a_fr, NB_MACHINES/4, 0);
	printf("\n");
	for(int i = 0; i < NB_MACHINES/4; i++){
		printf("machine_a_fr%d : ip = %s port = %d nom = %s \n", i, machine_a_fr[i].ip, machine_a_fr[i].port, machine_a_fr[i].nom);
	}
	

	servers_from_file("./lists/machine/b.fr", machine_b_fr, NB_MACHINES/4, 0);
	printf("\n");
	for(int i = 0; i < NB_MACHINES/4; i++){
		printf("machine_b_fr%d : ip = %s port = %d nom = %s \n", i, machine_b_fr[i].ip, machine_b_fr[i].port, machine_b_fr[i].nom);
	}
	

	servers_from_file("./lists/machine/a.com", machine_a_com, NB_MACHINES/4, 0);
	printf("\n");
	for(int i = 0; i < NB_MACHINES/4; i++){
		printf("machine_a_com%d : ip = %s port = %d nom = %s \n", i, machine_a_com[i].ip, machine_a_com[i].port, machine_a_com[i].nom);
	}
	

	servers_from_file("./lists/machine/b.com", machine_b_com, NB_MACHINES/4, 0);
	printf("\n");
	for(int i = 0; i < NB_MACHINES/4; i++){
		printf("machine_b_com%d : ip = %s port = %d nom = %s \n", i, machine_b_com[i].ip, machine_b_com[i].port, machine_b_com[i].nom);
	}
	
	

	// Création processus
	for(int j = 0; j < NB_SOUS_DOMAINES; j++){
		switch(fork()){
			case -1 :
				raler("fork", 1);
				break;
			case 0 : 
				if(j < 4)
					request_process(sousdomaine_fr[j].port, sousdomaine_fr[j].ip);
				else
					request_process(sousdomaine_fr[j].port, sousdomaine_fr[j].ip);
				break;
		}
	}


	n_wait(NB_SOUS_DOMAINES);
	return 0;
}