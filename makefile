programme: 
	make clean
	mkdir bin

	gcc -Wall -Wextra -o bin/client src/client.c src/fct_serveur.c
	gcc -Wall -Wextra -o bin/domaine src/domaine.c src/fct_serveur.c
	gcc -Wall -Wextra -o bin/sous_domaine src/sous_domaine.c src/fct_serveur.c
	gcc -Wall -Wextra -o bin/machine src/machine.c src/fct_serveur.c

clean:
	rm -rf bin/

init_ip:
	sudo ip addr add 192.168.0.200/24 dev wlp3s0
	sudo ip addr add 192.168.0.201/24 dev wlp3s0
	sudo ip addr add 192.168.0.202/24 dev wlp3s0
	sudo ip addr add 192.168.0.203/24 dev wlp3s0

	sudo ip addr add 2001::20 dev wlp3s0
	sudo ip addr add 2001::21 dev wlp3s0

	sudo ip addr add 192.168.0.210/24 dev wlp3s0 #processus domaine1
	sudo ip addr add 192.168.0.211/24 dev wlp3s0 #processus domaine2
	sudo ip addr add 192.168.0.212/24 dev wlp3s0 #processus domaine3

test:
	bash ./test.sh