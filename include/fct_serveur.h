#ifndef FCT_SERVEUR_H
#define FCT_SERVEUR_H 1

#define BUFFSIZE 1024

#define CLIENT_ADDR "2001::20"
#define CLIENT_PORT 44444

#define DOMAINE1_ADDR "::FFFF:192.168.0.210"
#define DOMAINE2_ADDR "::FFFF:192.168.0.211"
#define DOMAINE3_ADDR "::FFFF:192.168.0.212"
#define DOMAINE1_PORT 50010
#define DOMAINE2_PORT 50011
#define DOMAINE3_PORT 50012

#define SOUSDOMAINE_ADDR "::FFFF:192.168.0.202"
#define SOUSDOMAINE_PORT 50020

#define MACHINE_ADDR "::FFFF:192.168.0.203"
#define MACHINE_PORT 50030

void raler(char *, int );
int init_socket(struct sockaddr_in6*, long int, const char*, int, int);

int rcv(int);
int snd(int, const char *, struct sockaddr_in6 *);
#endif