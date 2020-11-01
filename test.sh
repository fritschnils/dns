echo "lancement serveur domaine"
gnome-terminal -- ./bin/domaine
#sleep 1
echo "lancement serveur sous_domaine"
gnome-terminal -- ./bin/sous_domaine
#sleep 1
echo "lancement serveur machine"
gnome-terminal -- ./bin/machine
#sleep 1
echo "lancement client et envoi des requetes ..."
./bin/client