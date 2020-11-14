echo "bash $ lancement serveur domaine"
gnome-terminal -- ./bin/domaine_resolver
#sleep 0.5
echo "bash $ lancement serveur sous_domaine"
gnome-terminal -- ./bin/sous_domaine_resolver
#sleep 0.5
echo "bash $ lancement serveur machine"
gnome-terminal -- ./bin/machine_resolver
sleep 0.1 # syncro des processus
echo "bash $ lancement client et envoi des requetes ..."
./bin/client -c