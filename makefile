programme: 
	make clean
	mkdir bin
	gcc -Wall -Wextra -o bin/client src/client.c src/fct_serveur.c
	gcc -Wall -Wextra -o bin/domaine src/domaine.c src/fct_serveur.c
	gcc -Wall -Wextra -o bin/sous_domaine src/sous_domaine.c src/fct_serveur.c
	gcc -Wall -Wextra -o bin/machine src/machine.c src/fct_serveur.c

clean:
	rm -rf bin/