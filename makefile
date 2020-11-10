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
	sudo ip -force addr add 192.168.0.200/24 dev wlp3s0 #processus client

	sudo ip -force addr add 192.168.0.211/24 dev wlp3s0 #processus racine1
	sudo ip -force addr add 192.168.0.212/24 dev wlp3s0 #processus racine2

	sudo ip -force addr add 192.168.0.221/24 dev wlp3s0 #processus server .fr
	sudo ip -force addr add 192.168.0.222/24 dev wlp3s0 #processus server .fr
	sudo ip -force addr add 192.168.0.223/24 dev wlp3s0 #processus server .com
	sudo ip -force addr add 192.168.0.224/24 dev wlp3s0 #processus server .com

	sudo ip -force addr add 192.168.0.231/24 dev wlp3s0 #processus server .a.fr
	sudo ip -force addr add 192.168.0.232/24 dev wlp3s0 #processus server .a.fr
	sudo ip -force addr add 192.168.0.233/24 dev wlp3s0 #processus server .b.fr
	sudo ip -force addr add 192.168.0.234/24 dev wlp3s0 #processus server .b.fr
	sudo ip -force addr add 192.168.0.235/24 dev wlp3s0 #processus server .a.com
	sudo ip -force addr add 192.168.0.236/24 dev wlp3s0 #processus server .a.com
	sudo ip -force addr add 192.168.0.237/24 dev wlp3s0 #processus server .b.com
	sudo ip -force addr add 192.168.0.238/24 dev wlp3s0 #processus server .b.com

test:
	bash ./test.sh