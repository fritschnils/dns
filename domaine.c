
#include <sys/types.h>	
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <limits.h>
#include <string.h>

void raler(char *msg, int perror_isset)
{
	fprintf(stderr, "%s\n", msg);
	if (perror_isset)
		perror(msg);
	exit(EXIT_FAILURE);
}

int main(int argc, char const *argv[])
{
	int sockfd, aff_port;
	ssize_t nb_octets;
	long int port = 50764;
	char buf[1024], aff_address[INET6_ADDRSTRLEN];
	
	socklen_t addrlen;
	struct sockaddr_in my_addr;
	struct sockaddr_in client;

	fd_set ens_read;


	if((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
		raler("socket", 1);

	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(port);
	my_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	addrlen = sizeof(struct sockaddr_in);
	memset(buf, '\0', 1024);
	if((bind(sockfd, (struct sockaddr *) &my_addr, addrlen)) == -1)
		raler("bind", 1);


	FD_ZERO(&ens_read);	
	FD_SET(sockfd, &ens_read);
	if((select(sockfd+1, &ens_read, NULL, NULL, NULL)) == -1)
		raler("select", 1);

	if(FD_ISSET(sockfd, &ens_read)){
		if((nb_octets = recvfrom(sockfd, buf, 1024, 0, (struct sockaddr *) &client, &addrlen)) == -1)
			raler("recvfrom", 1);



		if(sendto(sockfd, "bien recu\n", strlen("bien recu\n"), 0, (struct sockaddr *) &client, addrlen) == -1)
			raler("sendto", 1);

		if(inet_ntop(AF_INET, &client.sin_addr.s_addr,aff_address, INET6_ADDRSTRLEN) == NULL)
			raler("inet_ntop", 1);
		aff_port = ntohs(client.sin_port);
		printf("message reçu : %s\noctets reçus : %ld\nadresse expéditeur : %s\nport expéditeur : %d\n", buf, nb_octets, aff_address, aff_port);
	}

	if((close(sockfd)) == -1)
		raler("close", 1);

	return 0;
}