#include "socket.h"
#include <signal.h>

int creer_serveur ( int port ) {
	int socket_serveur = socket ( AF_INET , SOCK_STREAM , 0);
	if ( socket_serveur == -1) {
		perror ( " socket_serveur " );
		/* traitement de l ’ erreur */
	}
	int optval = 1;
	if ( setsockopt ( socket_serveur , SOL_SOCKET , SO_REUSEADDR , & optval , sizeof ( int )) == -1)
		perror ( " Can not set SO_REUSEADDR option " );
	/* Utilisation de la socket serveur */
	struct sockaddr_in saddr ;
	saddr.sin_family = AF_INET ; /* Socket ipv4 */
	saddr.sin_port = htons (port); /* Port d ’ écoute */
	saddr.sin_addr.s_addr = INADDR_ANY ; /* écoute sur toutes les interfaces */
	if ( bind ( socket_serveur , ( struct sockaddr *)& saddr , sizeof ( saddr )) == -1){
		perror ( " bind socker_serveur " ); /* traitement de l ’ erreur */
	}
	if ( listen ( socket_serveur , 10) == -1) {
		perror ( " listen socket_serveur " ); /* traitement d ’ erreur */
	}

	return socket_serveur;
}


void traitement_signal(int sig) {
	printf( "Signal %d reçu \n",sig);
}

void initialiser_signaux(void) {
	struct sigaction sa ;
	sa.sa_handler = traitement_signal ;
	sigemptyset(&sa.sa_mask );
	sa.sa_flags = SA_RESTART ;
	if ( sigaction ( SIGCHLD , &sa , NULL ) == -1) {
		perror ( "sigaction ( SIGCHLD )");
	}
}
