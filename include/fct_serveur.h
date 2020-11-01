#ifndef FCT_SERVEUR_H
#define FCT_SERVEUR_H 1

int init_socket_envoi(struct sockaddr_in *, long int );
int init_socket_reception(struct sockaddr_in *, long int , int );
void raler(char *, int );
#endif