#ifndef FCT_SERVEUR_H
#include "../include/fct_serveur.h"
#endif


socklen_t addrlen = sizeof(struct sockaddr_in6);

/* Fonction : Traiter les erreurs 
 * Arguments : 
 * 		- msg = message d'erreur 
 *		- perror_isset = true si perror est placé
 * Retour : rien
 */
void raler(char *msg, int perror_isset)
{
	fprintf(stderr, "%s\n", msg);
	if (perror_isset)
		perror(msg);
	exit(EXIT_FAILURE);
}

/* Fonction : Initialiser une socket 
 * Arguments : 
 * 		- adress = sockar_in à remplir
 *		- port = port de la socket
 *		- txt_addr = adresse de la socket (format string)
 *		- is_recv_socket = (=true) pour une socket de reception 
 *						   (=false) pour une socket d'envoi
 * Retour : Numéro du descripteur de la socket crée
 */
int init_socket(struct sockaddr_in6 *address, long int port, const char *txt_addr, int is_recv_socket)
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

/* Fonction : Attendre message sur une socket
 * Arguments : 
 * 		- sockfd = socket sur laquelle attendre
 * Retour : rien
 */
void rcv(int sockfd)
{
	char buf[BUFFSIZE];
	int nb_octets;

	memset(buf, '\0', BUFFSIZE);
	if((nb_octets = recvfrom(sockfd, buf, BUFFSIZE, 0, NULL, NULL)) == -1)
		raler("recvfrom", 1);

	//printf("message recu : %s\n", buf);

	if((close(sockfd)) == -1)
		raler("close", 1);	
}

/* Fonction : Envoyer un message
 * Arguments : 
 * 		- sockfd = socket utilisée pour envoyer 
 *		- msg = message à envoyer
 *		- dest_addr = adresse du destinataire
 * Retour : rien
 */
void snd(int sockfd, const char *msg, struct sockaddr_in6 *dest_adr)
{
	socklen_t addrlen = sizeof(struct sockaddr_in6);
	if(sendto(sockfd, msg, strlen(msg), 0, (struct sockaddr *) dest_adr, addrlen) == -1)
			raler("sendto", 1);

	if((close(sockfd)) == -1)
		raler("close", 1);	
}


/* Fonction : Récupérer une chaine depuis le format chaine|chaine|...|chaine
 *			  et la placer, bien formée ('\0' à la fin), dans storage.
 * Arguments : 
 * 		- fichier = structure de fichier dans lequel on cherche
 *		- storage = adresse où stocker la chaîne bien formée
 * Retour : Dernier charactère rencontré après la chaine
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

/* Fonction : Charger un fichier-liste de serveurs dans un tableau de serveurs.
 * Arguments : 
 * 		- filename = fichier dans lequel chercher les serveurs
 *		- serv_tab = tableau de serveurs à remplir
 *		- nb_lignes = nombre de lignes du fichier (= nombre de serveurs)
 * Retour : rien
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
				strncpy(serv_tab[i].nom, buf, 99);
			if(test == '|' && buf[0] != '.') // détecte une adresse ip
				strncpy(serv_tab[i].ip, buf, 39);
			if(test == '\n') //détecte un numéro de port
				serv_tab[i].port = atoi(buf);
		}
		i++;
		test = 0;
	}
	if(fclose(fichier) != 0)
		raler("fclose", 1);
}

/* Fonction : Attendre une requête de client et y répondre
 * Arguments : 
 * 		- port = port sur lequel attendre la requête
 *		- adresse = adresse sur laquelle attendre la requête
 * Retour : rien
 */
void request_process(int port, char *adresse)
{
	int sockfd;
	struct sockaddr_in6 my_addr, client_addr;

	// Initialisation - Reception - Fermeture ---------------------------------
	sockfd = init_socket(&my_addr, port, adresse, 1);	
	rcv(sockfd);
	// Initialisation - Envoi - Fermeture ---------------------------------
	sockfd = init_socket(&client_addr, CLIENT_PORT, CLIENT_ADDR, 0);
	snd(sockfd, DOMAINE_REQUEST, &client_addr);
	exit(EXIT_SUCCESS);
}

/* Fonction : Attendre n processus
 * Arguments : 
 * 		- n = nombre de processus à attendre
 * Retour : rien
 */
void n_wait(int n){
	int reason;

	for(int j = 0; j < n; j++){
		if(wait(&reason) == -1)
			raler("wait", 1);
		if(!WIFEXITED(reason) || WEXITSTATUS(reason) == EXIT_FAILS)
			raler("child terminated abnormally", 0);
	}
}