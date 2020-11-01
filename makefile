programme: 
	mkdir bin
	gcc -Wall -Wextra -o bin/client src/client.c
	gcc -Wall -Wextra -o bin/domaine src/domaine.c

clean:
	rm -rf bin/
