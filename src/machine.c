#ifndef FCT_SERVEUR_H
#include "../include/fct_serveur.h"
#endif

int main(int argc, char const *argv[])
{


	struct serveur machine_a_fr[4]; 
	struct serveur machine_a_com[4]; 
	struct serveur machine_b_fr[4]; 
	struct serveur machine_b_com[4]; 
	
	FILE* fichier = NULL;
	if((fichier = fopen("./lists/machine/a.fr", "r")) == NULL)
		raler("fopen", 1);

	servers_from_file(fichier, machine_a_fr, 4);

	if(fclose(fichier) != 0)
		raler("fclose", 1);


	for(int i = 0; i < 4; i++){
		printf("machine_a_fr%d :\nip = %s\nport = %d\nnom = %s\n", i, machine_a_fr[i].ip, machine_a_fr[i].port, machine_a_fr[i].nom);
	}





	fichier = NULL;
	if((fichier = fopen("./lists/machine/a.com", "r")) == NULL)
		raler("fopen", 1);

	servers_from_file(fichier, machine_a_com, 4);

	if(fclose(fichier) != 0)
		raler("fclose", 1);


	for(int i = 0; i < 4; i++){
		printf("machine_a_com%d :\nip = %s\nport = %d\nnom = %s\n", i, machine_a_com[i].ip, machine_a_com[i].port, machine_a_com[i].nom);
	}





	fichier = NULL;
	if((fichier = fopen("./lists/machine/b.fr", "r")) == NULL)
		raler("fopen", 1);

	servers_from_file(fichier, machine_b_fr, 4);

	if(fclose(fichier) != 0)
		raler("fclose", 1);


	for(int i = 0; i < 4; i++){
		printf("machine_b_fr%d :\nip = %s\nport = %d\nnom = %s\n", i, machine_b_fr[i].ip, machine_b_fr[i].port, machine_b_fr[i].nom);
	}





	fichier = NULL;
	if((fichier = fopen("./lists/machine/b.com", "r")) == NULL)
		raler("fopen", 1);

	servers_from_file(fichier, machine_b_com, 4);

	if(fclose(fichier) != 0)
		raler("fclose", 1);


	for(int i = 0; i < 4; i++){
		printf("machine_b_com%d :\nip = %s\nport = %d\nnom = %s\n", i, machine_b_com[i].ip, machine_b_com[i].port, machine_b_com[i].nom);
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
						machine_fils1();
				}
				break;

			case 1:
				switch(fork()){
					case -1 :
							raler("fork", 1);
						break;
					case 0 : 
						machine_fils2();
				}
				break;

			case 2:
				switch(fork()){
					case -1 :
						raler("fork", 1);
							break;
					case 0 : 
						machine_fils3();
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