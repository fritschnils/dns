# Préparer les adresses IP

1. Allez dans le makefile et entrez à la première ligne le nom de votre interface réseau à la place de "wlp3s0". Sauvegardez.
2. Entrez la commande suivante dans le terminal.
```console
make init_ip
```

# Compiler
Entrez la commande suivante dans le terminal
```console
make
```

# Lancer les tests
##### Test de robustesse : Différents noms à résoure avec fautes de syntaxe pour voir si le programme les détecte et quand.
En entrée : Un fichier contenant des noms tous non-valides.</br>
Résultat : Dans le fichier log, le retour du client. Normalement tous les noms sont inexistants.</br>
Remarque : Si vous voulez tester d'autres entrées, ajoutez les au fichier test/test_files/robustesse</br>
```console
make test-robustesse
```
##### Test de quantité : Mélange de plusieurs noms valides et non-valides pour voir si le programme se comporte bien.
En entrée : Un fichier contenant beaucoup de noms valides et non-valides</br>
Résultat : Dans le fichier log, le retour du client.</br>
Remarque : Si vous voulez tester d'autres entrées, ajoutez les au fichier test/test_files/quantite</br>
```console
make test-quantite
```
# Fin des Tests
Lancez cette commande pour enlever de votre interface les adresses utilisées
```console
    sudo service network-manager restart #pour remettre à zero
```