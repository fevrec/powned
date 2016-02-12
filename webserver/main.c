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

int main (void)
{	
	int socket_serveur = creer_serveur(8080);
	initialiser_signaux();	
	int statut;
	char *buf = malloc(sizeof(char)*80);
    //int alreadyDone = 0;
	while (1) {
		int socket_client = accept ( socket_serveur , NULL , NULL );
		if ( socket_client == -1){
			perror ( " accept " ); /* traitement d ’ erreur */
		}
		int pid;
		if ((pid = fork()) == 0) {
			/* On peut maintenant dialoguer avec le client */
			//sleep(1);
			/*const char * message_bienvenue = "Mesdames, messieurs,\n client, cliente\nJe suis heur
eux de vous annoncer que moi, serveur, s'est connecté à vous meme.\nPuisse le lien qui nous unis etre aussi fort que l'amour liant Mr java et Madame Eclipse.\nCe message sera envoyé tant que vous me recevrez, tant que vous m'ecouterez de votre oreille attentive...\nPaix et amour, guerre et haine, ne sont que futilitées, nous vivons notre passion comme un arbre perds ses feuilles en automne...\nVive les programmes ! Pro comme professionel, gramme comme le peu qu'il nous faut pour nous faire plaisir. Ajoutez a ces éléments, un peu de feuille marron et blanche afin d'obtenir un moment de paix et d'armonie autour de vous.\nA tout de suite pour un nouveau message !\n" ;
			write ( socket_client , message_bienvenue , strlen ( message_bienvenue ));
			*/
	FILE *f = fdopen(socket_client, "r+");
    while (fgets(buf, sizeof(buf),f) != '\0') {
      /*  if(alreadyDone==0){
           if(buf[0]=='G'&&buf[1]=='E'&&buf[2]=='T'){
                printf("oktamer");
                alreadyDone=1;
           }
           else{ 
                alreadyDone=-1;
                printf("c buger");
           }*/
	    printf("%s",buf);
        }
    
	fflush(stdout);
	write ( socket_client, buf , strlen (buf));
	free(buf);

			return 1;
		}
        printf("connexion acceptée\n") ;
		if (waitpid(pid, &statut, WNOHANG) == -1)
			perror("erreur waitpid");
            close(socket_client);
		
	}
   
	return 0;
}	
