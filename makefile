interface = wlp3s0

programme: 
	make clean
	mkdir bin

	gcc -Wall -Wextra -o bin/client src/client.c src/fct_serveur.c
	gcc -Wall -Wextra -o bin/domaine_resolver src/domaine_resolver.c src/fct_serveur.c
	gcc -Wall -Wextra -o bin/sous_domaine_resolver src/sous_domaine_resolver.c src/fct_serveur.c
	gcc -Wall -Wextra -o bin/machine_resolver src/machine_resolver.c src/fct_serveur.c

clean:
	rm -rf bin/

init_ip:
	sudo service network-manager restart #pour remettre à zero
	sleep 2
	sudo ip -force addr add 192.168.0.200/24 dev $(interface) #processus client

	sudo ip -force addr add 192.168.0.111/24 dev $(interface) #processus racine1
	sudo ip -force addr add 192.168.0.112/24 dev $(interface) #processus racine2

	sudo ip -force addr add 192.168.0.121/24 dev $(interface) #processus server .fr
	sudo ip -force addr add 192.168.0.122/24 dev $(interface) #processus server .fr
	sudo ip -force addr add 192.168.0.123/24 dev $(interface) #processus server .com
	sudo ip -force addr add 192.168.0.124/24 dev $(interface) #processus server .com

	sudo ip -force addr add 192.168.0.131/24 dev $(interface) #processus server .a.fr
	sudo ip -force addr add 192.168.0.132/24 dev $(interface) #processus server .a.fr
	sudo ip -force addr add 192.168.0.133/24 dev $(interface) #processus server .b.fr
	sudo ip -force addr add 192.168.0.134/24 dev $(interface) #processus server .b.fr
	sudo ip -force addr add 192.168.0.135/24 dev $(interface) #processus server .a.com
	sudo ip -force addr add 192.168.0.136/24 dev $(interface) #processus server .a.com
	sudo ip -force addr add 192.168.0.137/24 dev $(interface) #processus server .b.com
	sudo ip -force addr add 192.168.0.138/24 dev $(interface) #processus server .b.com

test-robustesse:
	bash ./test/test_scripts/test_robustesse.sh

test-quantite:
	bash ./test/test_scripts/test_quantite.sh