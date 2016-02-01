#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "socket.h"

int main (void)
{	
	int socket_serveur = creer_serveur(8080);
	int socket_client = accept ( socket_serveur , NULL , NULL );
	if ( socket_client == -1){
	perror ( " accept " ); /* traitement d ’ erreur */
	}
	/* On peut maintenant dialoguer avec le client */
	const char * message_bienvenue = "Bonjour , bienvenue sur mon serveur \n" ;
	write ( socket_client , message_bienvenue , strlen ( message_bienvenue ));
	return 0;
}	
