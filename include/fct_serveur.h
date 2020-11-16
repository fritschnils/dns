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
#include <sys/time.h>
#include <sys/stat.h>

#define BUFFSIZE 255
#define EXIT_FAILS 255

#define NB_RACINES 2
#define NB_DOMAINES 4
#define NB_SOUS_DOMAINES 8
#define NB_MACHINES 16

#define TYPE_RACINE 2
#define TYPE_SOUSDOMAINE 1
#define TYPE_MACHINE 0

#define CLIENT_ADDR "::FFFF:192.168.0.200"
#define CLIENT_PORT 44444

struct serveur{
	char nom[100];
	char ip[40];
	int port;
};

struct requete {
	char req[150];
	char nom[100];
	char ip[40];
	int port;
};

void raler(char *, int );
int init_socket(struct sockaddr_in6*, long int, char*, int);

void rcv(int, char*);
void snd(int, char *, struct sockaddr_in6 *);
int element_from_file(FILE *, char* );
void servers_from_file(char*, struct serveur*, int, int);
void request_process(int, char*, struct serveur*, int, int);
void n_wait(int);
void nom_from_request(char*, char *);
void domain_from_request(char* , char *, int);
void resolve(char *, struct serveur *, int , char *, int);


//FONCTIONS UNIQUEMENT POUR CLIENT
int nb_lignes(const char *);
void sitelist_from_file(const char *, struct requete *, int);
int init_client(struct requete **, const char *);
void timeval_to_str(struct timeval , char *);
void client_request_maker(char *, int , char *, char *);
int server_from_reponse(char *, struct serveur *, int);



#endif