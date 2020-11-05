# Simulateur de DNS


## Utilisation

- préparer les aresses ip : 
```console
make init_ip
```
- compilier avec commande : make 
- lancer les serveurs avec : make test

## Fonctionnalités

**Utilisation généralisée d'adresses ipv4 et ipv6. Serveurs joignables en ipv4 et ipv6** </br>
Passage des structures sockaddr_in en sockaddr_in6, AF_INET en PF_INET6(création socket) et AF_INET6(address familiy)
Passage des ipv4 de la forme X.X.X.X en ipv6 de la forme ::FFFF:X.X.X.X appelé *ipv4 mapped ipv6*