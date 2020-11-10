#ifndef FCT_SERVEUR_H
#include "../include/fct_serveur.h"
#endif



 
int main(int argc, char const *argv[])
{
	(void)argc;
	(void)argv;

	struct serveur racine[2]; // pour lui
	struct serveur sousdomaine[4]; // pour résoudre

	servers_from_file("./lists/infoclient", racine, 2); // pour lui


	servers_from_file("./lists/inforacine", sousdomaine, 4);


	
	for(int i = 0; i < 4; i++){
		printf("sousdomaine%d :\nip = %s\nport = %d\nnom = %s\n", i, sousdomaine[i].ip, sousdomaine[i].port, sousdomaine[i].nom);
	}
	
	/*
	int reason;

	for(int i=0;i<2;i++){
				switch(fork()){
					case -1 :
						raler("fork", 1);
						break;
					case 0 : 
						fils(racine[i].port, racine[i].ip);
						break;
				}
	}

	for(int j=0; j<2;j++){
		if(wait(&reason) == -1)
			raler("wait", 1);
		if(!WIFEXITED(reason) || WEXITSTATUS(reason) == EXIT_FAILS)
			raler("child terminated abnormally", 0);
	}
	exit(EXIT_SUCCESS);
	*/

	return 0;
}