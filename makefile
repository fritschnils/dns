programme: 
	mkdir bin
	gcc -Wall -Wextra -o bin/client src/client.c
	gcc -Wall -Wextra -o bin/domaine src/domaine.c
	gcc -Wall -Wextra -o bin/sous_domaine src/sous_domaine.c
	gcc -Wall -Wextra -o bin/machine src/machine.c

clean:
	rm -rf bin/