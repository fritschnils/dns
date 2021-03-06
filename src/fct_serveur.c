#ifndef FCT_SERVEUR_H
#include "../include/fct_serveur.h"
#endif




/**************************************************************************/
/* 						Variables globales                                */
/**************************************************************************/

socklen_t addrlen = sizeof(struct sockaddr_in6);





/**************************************************************************/
/* 						Fonctions Générales                               */
/**************************************************************************/

/* Fonction : Traiter les erreurs 
 * Arguments : 
 * 		- msg = message d'erreur 
 *		- perror_isset = true si perror est placé
 * Retour : rien
 */
void raler(char *msg, int perror_isset)
{
	if (perror_isset)
		perror(msg);
	else
		fprintf(stderr, "%s\n", msg);

	exit(EXIT_FAILURE);
}


/* Fonction : Savoir si une adresse est ipV4 
 * Arguments : 
 *		- adresseip = l'adresses ip testée
 * Retour : 0 si ipv6, sinon c'est ipv4
 */
int is_ipv4(char *adresseip)
{
    struct sockaddr_in sa;
    int result = inet_pton(AF_INET, adresseip, &(sa.sin_addr));
    return result != 0;
}


/* Fonction : Mapper une ipv4 en ipv6
 * Arguments : 
 * 		- ipv4 = ip à convertir
 *		- ipv6 = ip convertie
 * Retour : rien
 */
void ipv4toipv6(char *ipv4, char *ipv6)
{
	strncpy(ipv6, "::FFFF:", 7);
	strncpy(&ipv6[7], ipv4, 16);
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
int init_socket(struct sockaddr_in6 *address, long int port, char *txt_addr, int is_recv_socket)
{
	int sockfd, ip_bin;
	char ipv6[40];
	memset(ipv6, '\0', 40);
	if((sockfd = socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP)) == -1)
		raler("socket", 1);

	if(is_ipv4(txt_addr))
		ipv4toipv6(txt_addr, ipv6); // Convertit en ipv6-mapped ipv4

	address->sin6_family = AF_INET6;
	address->sin6_port = htons(port);
	if(ipv6[0] == '\0')
		ip_bin = inet_pton(AF_INET6, txt_addr, &address->sin6_addr);
	else 
		ip_bin = inet_pton(AF_INET6, ipv6, &address->sin6_addr);

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
 *		- storage = message reçu
 * Retour : rien
 */
void rcv(int sockfd, char *storage)
{
	int nb_octets;

	if((nb_octets = recvfrom(sockfd, storage, BUFFSIZE, 0, NULL, NULL)) == -1)
		raler("recvfrom", 1);

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
void snd(int sockfd, char *msg, struct sockaddr_in6 *dest_adr)
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

		if(!(c == '.' && i == 0)){
			storage[i] = c;
			i++;
		}
	}
}


/* Fonction : Charger un fichier-liste de serveurs dans un tableau de serveurs.
 * Arguments : 
 * 		- filename = fichier dans lequel chercher les serveurs
 *		- serv_tab = tableau de serveurs à remplir
 *		- nb_lignes = nombre de lignes du fichier (= nombre de serveurs)
 *		- isclient = (=true) si c'est le client qui appelle la fonction
 * Retour : rien
 */
void servers_from_file(char *filename, struct serveur *serv_tab, int nb_lignes, int isclient)
{
	char buf[100];
	int test, i = 0, isnewline = 1;

	FILE* fichier = NULL;
	if((fichier = fopen(filename, "r")) == NULL)
		raler("fopen", 1);

	while(i < nb_lignes){
		while(test != '\n'){
			memset(buf, '\0', 100);
			test = element_from_file(fichier, buf);

			if(test == '|' &&  isnewline && !isclient) //détecte un nom
				strncpy(serv_tab[i].nom, buf, 99);
			if(test == '|' && buf[0] != '.') //détecte une adresse ip
				strncpy(serv_tab[i].ip, buf, 39);
			if(test == '\n') //détecte un numéro de port
				serv_tab[i].port = atoi(buf);
			
			isnewline = 0;
		}
		isnewline = 1;
		i++;
		test = 0;
	}
	if(fclose(fichier) != 0)
		raler("fclose", 1);
}


/* Fonction : Extraire de la requete, la partie du nom à résoudre
 * Arguments : 
 * 		- storage = string où stocker la partie du nom à résoudre extraite
 *		- request = requete de laquelle extraire la partie du nom à résoudre
 *		- type = type du serveur appelant la fonction (sert à l'ocaliser la partie du nom à résoudre)
 * Retour : rien
 */
void domain_from_request(char* storage, char *request, int type)
{
	int repere = 2, i = 0, j = 0;

	while(repere){
		if(request[i] == '|')
			repere--;
		i++;
	}

	while(type){
		if(request[i] == '.')
			type--;
		i++;
	}

	while(request[i] != '\0'){
		storage[j] = request[i];
		i++;
		j++;
	}
	storage[j] = '\0';
}


/* Fonction : Extraire la partie nom de la requete
 * Arguments : 
 * 		- storage = string où stocker le nom extrait
 *		- request = requete de laquelle extraire le nom
 * Retour : rien
 */
void nom_from_request(char* storage, char *request)
{
	int repere = 2, i = 0, j = 0;
	while(repere){
		if(request[i] == '|')
			repere--;
		i++;
	}

	while(request[i] != '\0'){
		storage[j] = request[i];
		i++;
		j++;
	}
	storage[j] = '\0';
}


/* Fonction : Extraire la partie id-transaction de la requete
 * Arguments : 
 * 		- storage = string où stocker l'id-transaction extrait
 *		- request = requete de laquelle extraire l'id-transaction
 * Retour : rien
 */
void idtransac_from_request(char* storage, char *request)
{
	int i = 0;
	while(request[i] != '|'){
		storage[i] = request[i];
		i++;
	}
	storage[i] = '\0';
}


/* Fonction : Extraire la partie horodatage de la requete
 * Arguments : 
 * 		- storage = string où stocker l'horodatage extraite
 *		- request = requete de laquelle extraire l'horodatage
 * Retour : rien
 */
void horodatage_from_request(char* storage, char *request)
{
	int repere = 1, i = 0, j = 0;

	while(repere){
		if(request[i] == '|')
			repere--;
		i++;
	}

	while(request[i] != '|'){
		storage[j] = request[i];
		i++;
		j++;
	}
	storage[j] = '\0';
}


/* Fonction : Résoudre une requete client et construire la requete-réponse du processus-serveur appelant
 * Arguments : 
 * 		- brut_request = requete à résoudre sous forme brut (=comme reçue)
 *		- serv_resolution = base de donnée des resolutions du serveur
 * 		- taille = nombre de resolutions de la base de données du serveur
 *		- solution = string dans laquelle fabriquer la requete-réponse
 *		- server_type = type du serveur appelant (0=domaine_resolver, 1=sous-domaine_resolver ou 2=machine_resolver)
 * Retour : rien
 */
void resolve(char *brut_request, struct serveur *serv_resolution, int taille, char *solution, int server_type)
{
	int code = -1;
	
	char id_transac[11];
	char horodatage[11];
	char nom[BUFFSIZE];
	char triplet1[BUFFSIZE];//solution1
	char triplet2[BUFFSIZE];//solution2
	char a_resoudre[11];

	struct serveur solution1;
	struct serveur solution2;

	memset(solution1.nom, '\0', 100);
	memset(solution2.nom, '\0', 100);

	idtransac_from_request(id_transac, brut_request);
	horodatage_from_request(horodatage, brut_request);
	nom_from_request(nom, brut_request);
	domain_from_request(a_resoudre, brut_request, server_type);

	for(int i = 0; i < taille; i++){

		if(strcmp(serv_resolution[i].nom, a_resoudre) == 0){//si on trouve la resolution
			if(solution1.nom[0] == '\0'){//si s1 vide ecrire dedans sinon ecrire dans s2
				strncpy(solution1.nom, serv_resolution[i].nom, 99);
				solution1.port = serv_resolution[i].port;
				strncpy(solution1.ip, serv_resolution[i].ip, 40);
				code = 1; //si trouve au moins 1 solution alors code = 1
			}
			else{
				strncpy(solution2.nom, serv_resolution[i].nom, 99);
				solution2.port = serv_resolution[i].port;
				strncpy(solution2.ip, serv_resolution[i].ip, 40);
				code = 2; //si trouve 2 solutions alors code = 2
				break;//les deux solutions sont trouvée, on peut quitter
			}
		}
	}
	if(solution1.nom[0] == '\0' && solution2.nom[0] == '\0')
		code = 0;

	if(code == 1){
		sprintf(triplet1, "%s,%s,%d%c", solution1.nom, solution1.ip, solution1.port, '\0');
		sprintf(solution, "%s|%s|%s|%d|%s%c", id_transac, horodatage, nom, code, triplet1, '\0');
	}
	else if(code == 2){
		sprintf(triplet1, "%s,%s,%d%c", solution1.nom, solution1.ip, solution1.port, '\0');
		sprintf(triplet2, "%s,%s,%d%c", solution2.nom, solution2.ip, solution2.port, '\0');
		sprintf(solution, "%s|%s|%s|%d|%s|%s%c", id_transac, horodatage, nom, code, triplet1, triplet2, '\0');	
	}
	else if(code == 0){
		sprintf(solution, "%s|%s|%s|%d%c", id_transac, horodatage, nom, code, '\0');	
	}
	return;
}


/* Fonction : Attendre une requête de client et y répondre
 * Arguments : 
 * 		- port = port sur lequel attendre la requête
 *		- adresse = adresse sur laquelle attendre la requête
 *		- serv_resolution = liste des solutions de résolution
 *		- taille = nombre des solutions de résolution
 *		- server_type = type du serveur appelant (0=domaine_resolver, 1=sous-domaine_resolver ou 2=machine_resolver)
 * Retour : rien
 */
void request_process(int port, char *adresse, struct serveur *serv_resolution, int taille, int server_type)
{	

	char receive[BUFFSIZE];
	int sockfd_envoi, sockfd_reception;
	struct sockaddr_in6 my_addr, client_addr;
	char a_renvoyer[BUFFSIZE];
	char ack[2];

	while(1){

		memset(receive, '\0', BUFFSIZE);
		memset(a_renvoyer, '\0', BUFFSIZE);
		memset(ack, '\0', 2);

		//Reception
		sockfd_reception = init_socket(&my_addr, port, adresse, 1);	
		rcv(sockfd_reception, receive);
		if(receive[0] == '!')
			exit(EXIT_SUCCESS);

		//Resolution
		resolve(receive, serv_resolution, taille, a_renvoyer, server_type);
	
		//Envoi
		for(int x = 0; x < 600; x++){printf("~\n");}

		sockfd_envoi = init_socket(&client_addr, CLIENT_PORT, CLIENT_ADDR, 0);
		snd(sockfd_envoi, a_renvoyer, &client_addr);
		printf("envoi à client : %s\n", a_renvoyer);
	}
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








/**************************************************************************/
/* 						Fonctions Client                                  */
/**************************************************************************/


/* Fonction : Compter le nombre de sites à résoudre
 * Arguments : 
 * 		- filename = fichier contenant les sites à résoudre
 * Retour : Nombre de sites à résoudre
 */
int nb_lignes(const char *filename)
{
	int c;
	int nb_line = 0;
	FILE *fichier;

	if((fichier = fopen(filename, "r")) == NULL)
		raler("fopen", 1);

	while ((c = getc(fichier)) != EOF){
	    if (c == '\n')
	        ++nb_line;
	}

	if(fclose(fichier) != 0)
		raler("fclose", 1);
	return nb_line;
}


/* Fonction : Charger un fichier de sites à résoudre dans un tableau
 * Arguments : 
 * 		- filename = fichier contenant les sites à résoudre
 *		- req_tab = tableau dans lequel mettre les noms à résoudre
 *		- nb_requete = nombre de sites à résoudre
 * Retour : rien
 */
void sitelist_from_file(const char *filename, struct requete *req_tab, int nb_requete)
{
	char buf[100];
	int i = 0, j = 0;
	char* test;

	FILE* fichier = NULL;
	if((fichier = fopen(filename, "r")) == NULL)
		raler("fopen", 1);

	while(i < nb_requete){
		memset(buf, '\0', 100);
		test = fgets(buf, 99, fichier);
		if(test == NULL) //sécurité
			break;
		memset(req_tab[i].nom, '\0', 100);
		strncpy(req_tab[i].nom, buf, 99);

		while(req_tab[i].nom[j] != '\n')
			j++;
		req_tab[i].nom[j] = '\0';
		i++;
		j = 0;
	}

	if(fclose(fichier) != 0)
		raler("fclose", 1);
}


/* Fonction : Initialise les requetes initiales du client
 * Arguments : 
 * 		- tab = tableau dans lequel mettre les noms à résoudre
 *		- filename = nom du fichier à ouvrir
 * Retour : rien
 */
int init_client(struct requete **tab, const char* filename)
{
	int nb_sites = nb_lignes(filename);
	(*tab) = malloc(nb_sites*sizeof(struct requete));
	if(tab == NULL)
		raler("malloc requetes", 1);

	sitelist_from_file(filename, (*tab), nb_sites);
	return nb_sites;
}


/* Fonction : Convertir struct timeval en string de longueur 10
 * Arguments : 
 * 		- start = timeval à convertir
 *		- str = string dans laquelle convertir timeval
 * Retour : rien
 */
void timeval_to_str(struct timeval start, char *str)
{
    int nombre = start.tv_sec + start.tv_usec;
    sprintf(str, "%d%c", nombre, '\0');
}


/* Fonction : Fabriquer la requete client sous forme de string
 * Arguments : 
 * 		- storage = string où stocker la requete fabriquée
 *		- id-transac = id de transaction de la requete
 *		- horodatage = horodatage de la requete
 *		- nom = site de la requete (élément à résoudre)
 * Retour : rien
 */
void client_request_maker(char *storage, int id_transac, char *horodatage, char *nom)
{
	sprintf(storage, "%d|%s|%s%c", id_transac, horodatage, nom, '\0');
}


/* Fonction : Extrait le ou les infos des serveurs de la reponse resolue
 * Arguments : 
 * 		- reponse = reponse a la requete client
 *		- storage = struct serveur dans laquelle stocker les infos extraites
 *		- ismachine = (=true si c'est une reponse d'un machine_resolver) (=false sinon)
 * Retour : 1 si ok, -1 si site non trouvé (code == 0)
 */
int server_from_reponse(char *reponse, struct serveur *storage, int ismachine)
{
	int repere = 4, i = 0, j = 0;
	char port[6];
	
	//avance jusqu'au code et return -1 si code == 0
	while(repere-1){ 
		if(reponse[i] == '|')
			repere--;
		i++;
	}
	if(reponse[i] == '0')
		return -1;

	//avance jusqu'au premier triplet
	while(repere){ 
		if(reponse[i] == '|')
			repere--;
		i++;
	}

	
	for(int n = 0; n < 2; n++){ //pour maximum 2 solutions

		//recupere le nom du serveur
		while(reponse[i] != ','){
			storage[n].nom[j] = reponse[i];
			i++;
			j++;
		}
		storage[n].nom[j] = '\0';	
		i++;
		j = 0;
		//recupere l'ip du serveur
		while(reponse[i] != ','){
			storage[n].ip[j] = reponse[i];
			i++;
			j++;
		}
		storage[n].ip[j] = '\0';	
		i++;
		j = 0;
		
		//recupere le port du serveur
		while( (reponse[i] != '|' && n == 0 && !ismachine) || (reponse[i] != '\0' && n == 1) || (reponse[i] != '\0' && ismachine)){//recupere le port du serveur
			port[j] = reponse[i];
			i++;
			j++;
		}
		port[j] = '\0';
		storage[n].port = atoi(port);
		if(ismachine)
			return 1;
		i++;
		j = 0;
	}

	return 1;

}