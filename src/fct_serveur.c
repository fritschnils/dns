#ifndef FCT_SERVEUR_H
#include "../include/fct_serveur.h"
#endif


socklen_t addrlen = sizeof(struct sockaddr_in6);

void raler(char *msg, int perror_isset)
{
	fprintf(stderr, "%s\n", msg);
	if (perror_isset)
		perror(msg);
	exit(EXIT_FAILURE);
}

int init_socket(struct sockaddr_in6 *address, long int port, const char *txt_addr, int addrlen, int is_recv_socket)
{
	int sockfd, ip_bin;
	if((sockfd = socket(PF_INET6, SOCK_DGRAM, IPPROTO_UDP)) == -1)
		raler("socket", 1);

	address->sin6_family = AF_INET6;
	address->sin6_port = htons(port);
	ip_bin = inet_pton(AF_INET6, txt_addr, &address->sin6_addr);
	if(ip_bin == 0)
		raler("inet_pton : src does not contain a character string representing a valid network address in the specified address family", 0);
	if(ip_bin == -1)
		raler("inet_pton", 1);

	if(is_recv_socket){
		if((bind(sockfd, (struct sockaddr *) address, addrlen)) == -1)
			raler("bind", 1);
	}

	return sockfd;
}

int rcv(int sockfd)
{
	char buf[BUFFSIZE];
	int nb_octets;

	memset(buf, '\0', BUFFSIZE);
	if((nb_octets = recvfrom(sockfd, buf, BUFFSIZE, 0, NULL, NULL)) == -1)
		raler("recvfrom", 1);

	printf("message recu : %s\n", buf);

	if((close(sockfd)) == -1)
		raler("close", 1);	
	return 0;
}

int snd(int sockfd, const char *msg, struct sockaddr_in6 *client_addr)
{
	socklen_t addrlen = sizeof(struct sockaddr_in6);
	if(sendto(sockfd, msg, strlen(msg), 0, (struct sockaddr *) client_addr, addrlen) == -1)
			raler("sendto", 1);

	if((close(sockfd)) == -1)
		raler("close", 1);	
	return 0;
}

/*
 * Met dans <storage> la string (bien formée) reconnue jusqu'à avoir rencontré 'EOF' ou '|'' ou '\n'
 * Return : le dernier char rencontré (= EOF ou '|' ou '\n') 
 */
int element_from_file(FILE *fichier, char* storage)
{
	int c, i = 0;
	while(1){
		c = fgetc(fichier);
		if(c == EOF || c == '|' || c == '\n'){
			storage[i] = '\0';
			return c;
		}
		storage[i] = c;
		i++;
	}
}

/*
 * 
 * 
 */
void servers_from_file(char *filename, struct serveur *serv_tab, int nb_lignes)
{
	char buf[100];
	int test, i = 0;

	FILE* fichier = NULL;
	if((fichier = fopen(filename, "r")) == NULL)
		raler("fopen", 1);

	while(i < nb_lignes){
		while(test != '\n'){
			memset(buf, '\0', 100);
			test = element_from_file(fichier, buf);

			if(test == '|' && buf[0] == '.') // détecte un nom
				snprintf(serv_tab[i].nom, 99, buf);
			if(test == '|' && buf[0] != '.') // détecte une adresse ip
				snprintf(serv_tab[i].ip, 39, buf);
			if(test == '\n') //détecte un numéro de port
				serv_tab[i].port = atoi(buf);
		}
		i++;
		test = 0;
	}
	return;

	if(fclose(fichier) != 0)
		raler("fclose", 1);
}

void fils(int port, char *adresse)
{
	int sockfd;
	struct sockaddr_in6 my_addr, client_addr;

	// Initialisation - Reception - Fermeture ---------------------------------
	sockfd = init_socket(&my_addr, port, adresse, addrlen, 1);	
	rcv(sockfd);
	// Initialisation - Envoi - Fermeture ---------------------------------
	sockfd = init_socket(&client_addr, CLIENT_PORT, CLIENT_ADDR, addrlen, 0);
	snd(sockfd, DOMAINE_REQUEST, &client_addr);
	exit(EXIT_SUCCESS);
}


/*
void domaine_fils2()
{
	int sockfd;
	struct sockaddr_in6 my_addr, client_addr;

	// Initialisation - Reception - Fermeture ---------------------------------
	sockfd = init_socket(&my_addr, DOMAINE2_PORT, DOMAINE2_ADDR, addrlen, 1);	
	rcv(sockfd);
	// Initialisation - Envoi - Fermeture ---------------------------------
	sockfd = init_socket(&client_addr, CLIENT_PORT, CLIENT_ADDR, addrlen, 0);
	snd(sockfd, DOMAINE_REQUEST, &client_addr);
	exit(EXIT_SUCCESS);
}


void sousdomaine_fils1()
{
	int sockfd;
	struct sockaddr_in6 my_addr, client_addr;

	// Initialisation - Reception - Fermeture ---------------------------------
	sockfd = init_socket(&my_addr, SOUSDOMAINE1_PORT, SOUSDOMAINE1_ADDR, addrlen, 1);	
	rcv(sockfd);
	// Initialisation - Envoi - Fermeture ---------------------------------
	sockfd = init_socket(&client_addr, CLIENT_PORT, CLIENT_ADDR, addrlen, 0);
	snd(sockfd, SOUSDOMAINE_REQUEST, &client_addr);
	exit(EXIT_SUCCESS);
}

void sousdomaine_fils2()
{
	int sockfd;
	struct sockaddr_in6 my_addr, client_addr;

	// Initialisation - Reception - Fermeture ---------------------------------
	sockfd = init_socket(&my_addr, SOUSDOMAINE2_PORT, SOUSDOMAINE2_ADDR, addrlen, 1);	
	rcv(sockfd);
	// Initialisation - Envoi - Fermeture ---------------------------------
	sockfd = init_socket(&client_addr, CLIENT_PORT, CLIENT_ADDR, addrlen, 0);
	snd(sockfd, SOUSDOMAINE_REQUEST, &client_addr);
	exit(EXIT_SUCCESS);
}

void sousdomaine_fils3()
{
	int sockfd;
	struct sockaddr_in6 my_addr, client_addr;

	// Initialisation - Reception - Fermeture ---------------------------------
	sockfd = init_socket(&my_addr, SOUSDOMAINE3_PORT, SOUSDOMAINE3_ADDR, addrlen, 1);	
	rcv(sockfd);
	// Initialisation - Envoi - Fermeture ---------------------------------
	sockfd = init_socket(&client_addr, CLIENT_PORT, CLIENT_ADDR, addrlen, 0);
	snd(sockfd, SOUSDOMAINE_REQUEST, &client_addr);
	exit(EXIT_SUCCESS);
}


void machine_fils1()
{
	int sockfd;
	struct sockaddr_in6 my_addr, client_addr;

	// Initialisation - Reception - Fermeture ---------------------------------
	sockfd = init_socket(&my_addr, MACHINE1_PORT, MACHINE1_ADDR, addrlen, 1);	
	rcv(sockfd);
	// Initialisation - Envoi - Fermeture ---------------------------------
	sockfd = init_socket(&client_addr, CLIENT_PORT, CLIENT_ADDR, addrlen, 0);
	snd(sockfd, MACHINE_REQUEST, &client_addr);
	exit(EXIT_SUCCESS);
}

void machine_fils2()
{
	int sockfd;
	struct sockaddr_in6 my_addr, client_addr;

	// Initialisation - Reception - Fermeture ---------------------------------
	sockfd = init_socket(&my_addr, MACHINE2_PORT, MACHINE2_ADDR, addrlen, 1);	
	rcv(sockfd);
	// Initialisation - Envoi - Fermeture ---------------------------------
	sockfd = init_socket(&client_addr, CLIENT_PORT, CLIENT_ADDR, addrlen, 0);
	snd(sockfd, MACHINE_REQUEST, &client_addr);
	exit(EXIT_SUCCESS);
}

void machine_fils3()
{
	int sockfd;
	struct sockaddr_in6 my_addr, client_addr;

	// Initialisation - Reception - Fermeture ---------------------------------
	sockfd = init_socket(&my_addr, MACHINE3_PORT, MACHINE3_ADDR, addrlen, 1);	
	rcv(sockfd);
	// Initialisation - Envoi - Fermeture ---------------------------------
	sockfd = init_socket(&client_addr, CLIENT_PORT, CLIENT_ADDR, addrlen, 0);
	snd(sockfd, MACHINE_REQUEST, &client_addr);
	exit(EXIT_SUCCESS);
}
*/