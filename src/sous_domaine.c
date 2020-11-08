#ifndef FCT_SERVEUR_H
#include "../include/fct_serveur.h"
#endif

int main(int argc, char const *argv[])
{


	struct serveur sousdomaine_fr[4]; 
	struct serveur sousdomaine_com[4]; 

	FILE* fichier = NULL;
	if((fichier = fopen("./lists/sousdomaine/fr", "r")) == NULL)
		raler("fopen", 1);

	servers_from_file(fichier, sousdomaine_fr, 4);

	if(fclose(fichier) != 0)
		raler("fclose", 1);


	for(int i = 0; i < 4; i++){
		printf("sousdomaine_fr%d :\nip = %s\nport = %d\nnom = %s\n", i, sousdomaine_fr[i].ip, sousdomaine_fr[i].port, sousdomaine_fr[i].nom);
	}
	






	fichier = NULL;
	if((fichier = fopen("./lists/sousdomaine/com", "r")) == NULL)
		raler("fopen", 1);

	servers_from_file(fichier, sousdomaine_com, 4);

	if(fclose(fichier) != 0)
		raler("fclose", 1);


	for(int i = 0; i < 4; i++){
		printf("sousdomaine_com%d :\nip = %s\nport = %d\nnom = %s\n", i, sousdomaine_com[i].ip, sousdomaine_com[i].port, sousdomaine_com[i].nom);
	
	}
/*
	int reason;

	for(int i=0;i<3;i++){
		switch(i){
			case 0:
				switch(fork()){
					case -1 :
						raler("fork", 1);
						break;
					case 0 : 
						sousdomaine_fils1();
				}
				break;

			case 1:
				switch(fork()){
					case -1 :
							raler("fork", 1);
						break;
					case 0 : 
						sousdomaine_fils2();
				}
				break;

			case 2:
				switch(fork()){
					case -1 :
						raler("fork", 1);
							break;
					case 0 : 
						sousdomaine_fils3();
				}
				break;
		}
	}

	for(int j=0; j<3;j++){
		if(wait(&reason) == -1)
			raler("wait", 1);
		if(!WIFEXITED(reason) || WEXITSTATUS(reason) == EXIT_FAILS)
			raler("child terminated abnormally", 0);
	}
	exit(EXIT_SUCCESS);
	*/
	return 0;
}