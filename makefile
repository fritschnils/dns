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
	sudo service network-manager restart
	sleep 2
	sudo ip -force addr add 192.168.0.200/24 dev wlp3s0 #processus client

	sudo ip -force addr add 192.168.0.211/24 dev wlp3s0 #processus domaine1
	sudo ip -force addr add 192.168.0.212/24 dev wlp3s0 #processus domaine2

	sudo ip -force addr add 192.168.0.221/24 dev wlp3s0 #processus sous_domaine1
	sudo ip -force addr add 192.168.0.222/24 dev wlp3s0 #processus sous_domaine2
	sudo ip -force addr add 192.168.0.223/24 dev wlp3s0 #processus sous_domaine3
	sudo ip -force addr add 192.168.0.224/24 dev wlp3s0 #processus sous_domaine4

	sudo ip -force addr add 192.168.0.231/24 dev wlp3s0 #processus machine1
	sudo ip -force addr add 192.168.0.232/24 dev wlp3s0 #processus machine2
	sudo ip -force addr add 192.168.0.233/24 dev wlp3s0 #processus machine3
	sudo ip -force addr add 192.168.0.234/24 dev wlp3s0 #processus machine4
	sudo ip -force addr add 192.168.0.235/24 dev wlp3s0 #processus machine5
	sudo ip -force addr add 192.168.0.236/24 dev wlp3s0 #processus machine6
	sudo ip -force addr add 192.168.0.237/24 dev wlp3s0 #processus machine7
	sudo ip -force addr add 192.168.0.238/24 dev wlp3s0 #processus machine8

test:
	bash ./test.sh