#include <stdio.h>
#include <string.h>
#include "socket.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

int main (/*int argc,char ** argv*/void)
{
  printf("Creation du serveur\n");

  int serveur = creer_serveur(8080);
  //int socket_client;

 if(serveur != -1){
   sleep(1);
   printf("Le serveur a ete cree !\n");
  }

 start(serveur);

 
 //close(serveur);

 //Traitement de la socket client !
 
 /*socket_client = accept(serveur, NULL, NULL);
 
 if(socket_client == -1){
   perror("Erreur lors de la connexion a la socket cliente");
 }
 
 else if(serveur == -1){
    printf("Ca marche pas !\n");
  }

 close(socket_client);
 */
  return 0;
}
