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
char * fgets_or_exit(char * buf , int size , FILE * stream ){
//VARIABLE 
	char* e400="HTTP/1.1 400 Bad Request\r\nConnection: close\r\nContent-Length: 17\r\n\r\n400 Bad request\r\n";
	char* w200="HTTP/1.1 200 OK\r\nConnection: close\r\nContent-Length: 17\r\n\r\nSalut poto ! :)\r\n";
	char* e404="HTTP/1.1 404 Not Found\r\nConnection: close\r\nContent-Length: 15\r\n\r\n404 Not Found\r\n";
	http_request request;
	fgets(buf, size,stream);
	int erreur = parse_http_request(buf,&request);
	if(strcmp(request.url,"/") != 0)
		return e404;
    	else if(erreur == 400)
		return e400;
	else
		return w200;
		
}

int parse_http_request(const char * request_line, http_request * request){
	//4. À l’aide de la RFC, trouvez le nom donné aux trois constituant de la première ligne de la
	//requête envoyée par le client.
	/*int getDone = 0;
	int httpDone = 0;
	int troisMot = 0;
	for(i=0;i<15;i++){
		if(troisMot == 0){
			if(cpt!=3 && buf[i]!='\n'){
				if(buf[i]==' ')
					cpt++;
			}
			else{ 
				if(cpt==3){
					troisMot=1;
					printf("3MOTS_OK\n");
				}
				else{
					troisMot = -1;
					printf("Erreur : Il n'y a pas 3 mots sur la premiere ligne\n");
				}
			}
		}
	}
	if(getDone == 0){
        if(buf[0]=='G'&&buf[1]=='E'&&buf[2]=='T'){
            printf("GET_OK\n");
            getDone=1;
        }
        else{ 
            getDone=-1;
            printf("GET_ERROR\n");
        }
		if (buf[5] != ' ')
			error404 = 1;			
	}
	if(httpDone == 0){
		if(buf[6]=='H'&&buf[7]=='T'&&buf[8]=='T'&&buf[9]=='P'&&buf[10]=='/'&&buf[11]=='1'&&buf[12]=='.'&&(buf[13]=='0'||buf[13]=='1')){
			printf("HTTP_OK\n");
			httpDone=1;
		}
		else {
			printf("HTTP_ERROR");
			httpDone=-1;
		}
	}*/

	int i = 0;
	int taille = strlen(request_line);
	int cpt = 0;
	int cpt2 = 0;
	char ** ligne = malloc(taille);
	//printf("%s",request_line);
	printf("%d\n",taille);
	request->url = "/";
	while(i<taille && (request_line[i] != '\r' || request_line[i] != '\n')) {
		if(request_line[i] == ' ') {
			cpt2++;
			cpt = 0;
		}
		else {
			ligne[cpt2][cpt] = request_line[i];
			cpt++;
		}
		printf("%d\n",i);
		i++;
	}
	printf("-----------------------------\n");
	printf("%d\n",i);
	printf("-----------------------------\n");
	return 0;
}



















