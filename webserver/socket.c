#include <stdio.h>
#include "socket.h" 
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>


int creer_serveur(int port){

  /** Parametres lies a la structure */

  struct sockaddr_in saddr;

  saddr.sin_family = AF_INET; /* Socket ipv4 */
  saddr.sin_port = htons(port); /* Port d'ecoute*/
  saddr.sin_addr.s_addr = INADDR_ANY; /* ecoute sur toutes les interfaces*/
  
  int socket_serveur;
  socket_serveur = socket(AF_INET, SOCK_STREAM, 0);

  
  /** Traitement du cas d'erreur */
  if(socket_serveur == -1){
    perror("Erreur lors de la creation de la socket serveur\n");
  }
  
  /** Traitement du bind avec le cas d'erreur associe */
  if(bind(socket_serveur, (struct sockaddr *) &saddr, sizeof(saddr)) == -1){
    perror("Erreur lors du binding de la socket serveur\n");
  }

  /** Traitement du listening qui lance l'attente de connexion avec le cas 
   d'erreur */
  if(listen(socket_serveur, 10) == -1){
    perror("Erreur lors de l'attente de connexion de la socket serveur\n");
  }

  /** Accepter une connexion */
 

  /** Ecriture du message de bienvenue */

  /**Fermeture de la socket serveur */
  //close(socket_serveur);
  
  return socket_serveur;
}

int start(int socket_serveur){

    int socket_client;
    socket_client = accept(socket_serveur, NULL, NULL);
    if(socket_client == -1){
      perror("Erreur lors de la connexion du client\n");
    }

    
    const char *message_bienvenue = "Welcome to the jungle !\n";
    sleep(1);

    
    write(socket_client, message_bienvenue, strlen(message_bienvenue));
    return socket_client;
  }

