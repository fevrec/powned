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
	char *buf = malloc(255);
   	int getDone = 0;
	int httpDone = 0;
	int troisMot = 0;
	int cpt = 0;
	char* e400="HTTP/1.1 400 Bad Request\r\nConnection: close\r\nContent-Length: 17\r\n\r\n400 Bad request\r\n";
	char* w200="HTTP/1.1 200 OK\r\nConnection: close\r\nContent-Length: 17\r\n\r\nSalut poto ! :)\r\n";
	
	while (1) {
		int socket_client = accept ( socket_serveur , NULL , NULL );
		if ( socket_client == -1){
			perror ( " accept " ); /* traitement d ’ erreur */
		}
		int pid;
		if ((pid = fork()) == 0) {
			int i;
			/* On peut maintenant dialoguer avec le client */
			//sleep(1);
			/*const char * message_bienvenue = "Mesdames, messieurs,\n client, cliente\nJe suis heureux de vous annoncer que moi, serveur, s'est connecté à vous meme.\nPuisse le lien qui nous unis etre aussi fort que l'amour liant Mr java et Madame Eclipse.\nCe message sera envoyé tant que vous me recevrez, tant que vous m'ecouterez de votre oreille attentive...\nPaix et amour, guerre et haine, ne sont que futilitées, nous vivons notre passion comme un arbre perds ses feuilles en automne...\nVive les programmes ! Pro comme professionel, gramme comme le peu qu'il nous faut pour nous faire plaisir. Ajoutez a ces éléments, un peu de feuille marron et blanche afin d'obtenir un moment de paix et d'armonie autour de vous.\nA tout de suite pour un nouveau message !\n" ;
			write ( socket_client , message_bienvenue , strlen ( message_bienvenue ));*/
			
			FILE *f = fdopen(socket_client, "w+");
	 		
   			// while (fgets(buf, sizeof(char)*80,f) != '\0') {
			while (1) {
				buf = malloc(255);
				fgets(buf, 255,f);
				for(i=0;i<15;i++){
					if(troisMot == 0){
						if(cpt!=3 && buf[i]!='\n'){
							if(buf[i]==' '){
								cpt++;
							}	
						}
						else if(cpt==3){
							troisMot=1;
							printf("3MOTS_OK\n");
						}
						else{
							troisMot = -1;
							printf("Erreur : Il n'y a pas 3 mots sur la premiere ligne\n");
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
				}
	   // printf("%s",buf);
			if (strcmp(buf, "\r\n") == 0)
	  			break;
			if(strcmp(buf, "") == 0)
	  			break;
   			}
			
    			if(getDone == -1 || httpDone == -1 || troisMot==-1){
				write ( socket_client , e400 , strlen ( e400 ));
			}
			else 
				write ( socket_client , w200 , strlen ( w200 ));
			
			free(buf);
			fclose(f);
			return 1;
		}
       		 printf("connexion acceptée\n") ;
		if (waitpid(pid, &statut, WNOHANG) == -1)
			perror("erreur waitpid");
            	close(socket_client);
		
	}
	return 0;
}	

