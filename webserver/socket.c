#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include "socket.h"
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
	int pid;
	int status;
	while ((pid=waitpid(-1, &status, WNOHANG)) != -1)
		kill(pid,SIGKILL);
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
void fgets_or_exit(char * buf , int size , FILE * stream ){
//VARIABLE 
	/*char* e400="HTTP/1.1 400 Bad Request\r\nConnection: close\r\nContent-Length: 17\r\n\r\n400 Bad request\r\n";
	char* w200="HTTP/1.1 200 OK\r\nConnection: close\r\nContent-Length: 17\r\n\r\nSalut poto ! :)\r\n";
	char* e404="HTTP/1.1 404 Not Found\r\nConnection: close\r\nContent-Length: 15\r\n\r\n404 Not Found\r\n";*/
	http_request request;
	fgets(buf, size,stream);

	char * request_line = malloc(size);
	strcpy(request_line, buf);
	skip_headers (stream , size);
	int bad_request = parse_http_request(request_line,&request);

	if ( bad_request )
		send_response ( stream , 400 , "Bad Request" , "Bad request\r\n" );
	else if (request.method == HTTP_UNSUPPORTED )
		send_response ( stream , 405 , "Method Not Allowed" , "Method Not Allowed\r\n" );
	else if (strcmp(request.url , "/" ) == 0)
		send_response ( stream , 200 , "OK" , "Salut poto ! :)\r\n" );
	else
		send_response ( stream , 404 , "Not Found" , "Not Found\r\n" );
}

int parse_http_request(char * request_line, http_request * request){
	char ** troisMot = malloc(strlen(request_line));
	char * partie;
	int cpt = 0;
	const char s[2] = " ";
	   /* get the first token */
	partie = strtok(request_line, s);
	   
	   /* walk through other tokens */
	while( partie != NULL )  {
	    troisMot[cpt] = partie;
	    cpt++;
	    partie = strtok(NULL, s);
	};
	if(strcmp(troisMot[0], "GET") == 0)
		request->method = HTTP_GET;
	else
		request->method = HTTP_UNSUPPORTED;

	request->url = troisMot[1];
	
	request->major_version = troisMot[2][5]-'0';
	request->minor_version = troisMot[2][7]-'0';

	if(request->major_version == 1 && (request->minor_version == 1 || request->minor_version == 0))
		return 0; 
	return 1;
}

void skip_headers ( FILE * client , int size) {
	char * ligne = malloc(size);
	do {
		fgets(ligne, size ,client);
	}while(strcmp(ligne,"\r\n") != 0 && strcmp(ligne,"\n") != 0);
}

void send_status ( FILE * client , int code , const char * reason_phrase ) {
	char * phrase = malloc(50);
	sprintf(phrase, "HTTP/1.1 %d %s\r\n", code , reason_phrase);
	write ( 1 , phrase , strlen (phrase));
	fputs(phrase,client);
}

void send_response ( FILE * client , int code , const char * reason_phrase ,const char * message_body ) {
	send_status(client, code, reason_phrase);
	char * phrase = malloc(50);
	sprintf(phrase, "Connection: close\r\nContent-Length: %zu \r\n\r\n%s",strlen(message_body),message_body);	
	write ( 1 , phrase , strlen (phrase));
	fputs(phrase,client);
}













