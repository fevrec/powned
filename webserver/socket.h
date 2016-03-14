#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>


enum http_method {
	HTTP_GET ,
	HTTP_UNSUPPORTED ,
};

typedef struct
{
	enum http_method method ;
	int major_version ;
	int minor_version ;
	char * url ;
} http_request ;

# ifndef __SOCKET_H__
# define __SOCKET_H__
/* * Crée une socket serveur qui écoute sur toute les interfaces IPv4
de la machine sur le port passé en paramètre . La socket retournée
doit pouvoir être utilisée directement par un appel à accept .
La fonction retourne -1 en cas d ’ erreur ou le descripteur de la
socket créée . */
int creer_serveur ( int port );
void initialiser_signaux ( void );
void traitement_signal ( int sig );

# endif
char * fgets_or_exit ( char * buf , int size , FILE * stream );
int parse_http_request(const char * request_line, http_request * request);


