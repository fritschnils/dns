#ifndef FCT_SERVEUR_H
#define FCT_SERVEUR_H 1

#define BUFFSIZE 1024
#define CLIENT_ADDR "2001::20"
#define DOMAINE_ADDR "2001::21"
#define SOUSDOMAINE_ADDR "::FFFF:192.168.0.202"
#define MACHINE_ADDR "::FFFF:192.168.0.203"

void raler(char *, int );
int init_socket(struct sockaddr_in6*, long int, const char*, int, int);
#endif