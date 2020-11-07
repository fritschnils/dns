#ifndef FCT_SERVEUR_H
#define FCT_SERVEUR_H 1

#include <sys/types.h>	
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <limits.h>
#include <string.h>
#include <sys/wait.h>


#define BUFFSIZE 1024
#define EXIT_FAILS 255
#define DOMAINE_REQUEST "domaine recu"
#define SOUSDOMAINE_REQUEST "sous_domaine recu"
#define MACHINE_REQUEST "machine recu"

#define CLIENT_ADDR "::FFFF:192.168.0.200"
#define CLIENT_PORT 44444

#define DOMAINE1_ADDR "::FFFF:192.168.0.211"
#define DOMAINE2_ADDR "::FFFF:192.168.0.212"
#define DOMAINE3_ADDR "::FFFF:192.168.0.213"
#define DOMAINE1_PORT 50011
#define DOMAINE2_PORT 50012
#define DOMAINE3_PORT 50013

#define SOUSDOMAINE1_ADDR "::FFFF:192.168.0.221"
#define SOUSDOMAINE2_ADDR "::FFFF:192.168.0.222"
#define SOUSDOMAINE3_ADDR "::FFFF:192.168.0.223"
#define SOUSDOMAINE1_PORT 50021
#define SOUSDOMAINE2_PORT 50022
#define SOUSDOMAINE3_PORT 50023

#define MACHINE1_ADDR "::FFFF:192.168.0.231"
#define MACHINE2_ADDR "::FFFF:192.168.0.232"
#define MACHINE3_ADDR "::FFFF:192.168.0.233"
#define MACHINE1_PORT 50031
#define MACHINE2_PORT 50032
#define MACHINE3_PORT 50033

void raler(char *, int );
int init_socket(struct sockaddr_in6*, long int, const char*, int, int);

int rcv(int);
int snd(int, const char *, struct sockaddr_in6 *);

// Fonctions domaine
void domaine_fils1();
void domaine_fils2();
void domaine_fils3();

// Fonctions sous_domaine
void sousdomaine_fils1();
void sousdomaine_fils2();
void sousdomaine_fils3();

// Fonctions machine
void machine_fils1();
void machine_fils2();
void machine_fils3();
#endif