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

	struct serveur sousdomaine_fr[NB_SOUS_DOMAINES/2];
	struct serveur sousdomaine_com[NB_SOUS_DOMAINES/2];

	struct serveur machine_a_fr[NB_MACHINES/4];
	struct serveur machine_b_fr[NB_MACHINES/4];
	struct serveur machine_a_com[NB_MACHINES/4];
	struct serveur machine_b_com[NB_MACHINES/4];

	//Adresses et ports pour les serveurs sous-domaine (serveurs lancés dans ce programme)
	servers_from_file("./lists/sousdomaine/fr", sousdomaine_fr, NB_SOUS_DOMAINES/2, 0);
	servers_from_file("./lists/sousdomaine/com", sousdomaine_com, NB_SOUS_DOMAINES/2, 0);

	//Chaque type de serveur a accès à son propre fichier de résolution des machines
	servers_from_file("./lists/machine/a.fr", machine_a_fr, NB_MACHINES/4, 0);
	servers_from_file("./lists/machine/b.fr", machine_b_fr, NB_MACHINES/4, 0);
	servers_from_file("./lists/machine/a.com", machine_a_com, NB_MACHINES/4, 0);
	servers_from_file("./lists/machine/b.com", machine_b_com, NB_MACHINES/4, 0);
	
	

/**************************************************************************/
/* Lancement des serveurs. 1 serveur par processus et chacun sa socket    */
/**************************************************************************/
	for(int j = 0; j < NB_MACHINES/2; j++){
		switch(fork()){
			case -1 :
				raler("fork", 1);
				break;
			case 0 : 
				if(j < 2){
					request_process(sousdomaine_fr[j].port, sousdomaine_fr[j].ip, machine_a_fr, NB_MACHINES/4, TYPE_MACHINE);
					exit(EXIT_SUCCESS);
				}
				else if(j < 4){
					request_process(sousdomaine_fr[j].port, sousdomaine_fr[j].ip, machine_b_fr, NB_MACHINES/4, TYPE_MACHINE);
					exit(EXIT_SUCCESS);
				}
				else if(j < 6){
					request_process(sousdomaine_com[j%4].port, sousdomaine_com[j%4].ip, machine_a_com, NB_MACHINES/4, TYPE_MACHINE);
					exit(EXIT_SUCCESS);
				}
				else{
					request_process(sousdomaine_com[j%4].port, sousdomaine_com[j%4].ip, machine_b_com, NB_MACHINES/4, TYPE_MACHINE);
					exit(EXIT_SUCCESS);
				}
				break;
		}
	}
	n_wait(NB_MACHINES/2);
	return 0;
}