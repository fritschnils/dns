#ifndef FCT_SERVEUR_H
#define FCT_SERVEUR_H 1

#define BUFFSIZE 1024
#define CLIENT_ADDR "192.168.0.200"
#define DOMAINE_ADDR "192.168.0.201"
#define SOUSDOMAINE_ADDR "192.168.0.202"
#define MACHINE_ADDR "192.168.0.203"

void raler(char *, int );
int init_socket_reception(struct sockaddr_in *, long int, int);
int init_socket_envoi(struct sockaddr_in *, long int );
int init_socket(struct sockaddr_in*, long int, const char*, int, int);
#endif