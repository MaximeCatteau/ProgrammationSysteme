#include <stdio.h>
#include "socket.h" 
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <ctype.h>


int socket_client;

/** Ignorer SIGPIPE */

void traitement_signal(int sig){

  printf("Signal %d recu\n", sig);
  waitpid(-1, NULL, 0);
  
}

void initialiser_signaux(void){

  
  //Avant
  /*if(signal(SIGPIPE, SIG_IGN) == SIG_ERR){
    perror("Erreur dans le SIGPIPE\n");
    }*/

  //Apres

  struct sigaction sa;

  sa.sa_handler = traitement_signal;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_RESTART;
  
  if(sigaction(SIGCHLD, &sa, NULL) == -1){
    perror("Erreur dans le sigaction(SIGCHLD)\n");
  }
}

int creer_serveur(int port){
  
  /** SIGPIPE */
  initialiser_signaux();
  
  /** Parametres lies a la structure */
  struct sockaddr_in saddr;

  saddr.sin_family = AF_INET; /* Socket ipv4 */
  saddr.sin_port = htons(port); /* Port d'ecoute*/
  saddr.sin_addr.s_addr = INADDR_ANY; /* ecoute sur toutes les interfaces*/
  
  int socket_serveur;
  socket_serveur = socket(AF_INET, SOCK_STREAM, 0);

  /** Pour pouvoir se reconnecter apres avoir quitte le serveur */

  int optval = 1;

  if(setsockopt(socket_serveur, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int)) == -1){
    perror("Ne peut pas etablir l'option SO_REUSEADDR\n");
  }

  
  /** Traitement du cas d'erreur */
  if(socket_serveur == -1){
    perror("Erreur lors de la creation de la socket serveur\n");
  } else { printf("CREATION SOCKET SERVEUR OK\n"); }
  
  /** Traitement du bind avec le cas d'erreur associe */
  if(bind(socket_serveur, (struct sockaddr *) &saddr, sizeof(saddr)) == -1){
    perror("Erreur lors du binding de la socket serveur\n");
  } else { printf("BINDING OK\n"); }

  /** Traitement du listening qui lance l'attente de connexion avec le cas 
   d'erreur */
  if(listen(socket_serveur, 10) == -1){
    perror("Erreur lors de l'attente de connexion de la socket serveur\n");
  } else { printf("LISTENING OK\n");}

  
  return socket_serveur;
}


/** Creer une connexion avec un client */

int start(int socket_serveur){
  
  while(1){
    //int socket_client;
    int pid;
    socket_client = accept(socket_serveur, NULL, NULL);
    
    if(socket_client == -1){
      perror("Erreur lors de la connexion du client\n");
    } else { printf("CONNEXION CLIENT OK\n");}

    FILE *fclient = fdopen(socket_client, "w+");
    
    const char *message_bienvenue = "Welcome !\nYou'll find here the best server of N4P2-1 by Maxime Catteau\n";
    sleep(1);

    fprintf(fclient, message_bienvenue);
    //write(socket_client, message_bienvenue, strlen(message_bienvenue));

    int buffer_size = 4096;
    int mark = 1;

    if((pid = fork()) == 0){
      while(mark == 1){
	char *buffer = calloc(buffer_size, 1);
        char * ligne = "";
	char *ptr;
	int i;
	int mots = 0;
	ligne = fgets(buffer, buffer_size, fclient);
	int taille = strlen(ligne);
	
	if(strncmp("GET", ligne, 3)== 0){
	   if(strcmp(strstr(ligne,"HTTP/"), "HTTP")){
	    printf("HTTP found\n");
	    if( (ptr = strrchr(ligne, '/')) != NULL){

	      //49 est la valeur ascii de 1
	      if(ligne[ptr - ligne + 1] == 49){
		printf("%d\n", ligne[ptr - ligne +1]);
		printf("Le premier chiffre M vaut bien 1\n");

		if((ptr = strrchr(ligne, '.')) != NULL){
		  if(ligne[ptr - ligne + 1] == 48 ||ligne[ptr - ligne + 1] == 49){
		    printf("Le deuxieme chiffre vaut 0 ou 1\n");
		    
		    printf("HTTP/1.1 200 OK\r\nContent-Length: %d\n", (int) sizeof(ligne));
		  }
		}
	      }
	    }	   
	  }
	  for( i = 0; i < taille; i++){
	    if(isspace(ligne[i]) != 0 || ligne[i] == '\n'){
	      mots++;
	    }
	  }

	  if(strncmp("\r\n", ligne, 0) != 0 || strncmp("\n", ligne, 0) != 0){
	    printf("<Pawnee> %s", ligne);     
	  }
	  if(mots-1 == 3){
	    printf("GET => OK\n");
	  }

	  else{
	    printf("HTTP/1.1 400 Bad Request\r\n Connection: close\r\nContent-Length: 17\r\n\r\n400 Bad request\r\n");
	  }
	}


    
	//fprintf(fclient, "<Pawnee> ");
	//fprintf(fclient, buffer);
	
	/*
	  read(socket_client, buffer, buffer_size);
	  
	  write(socket_client, buffer, buffer_size);
	*/

	//QUESTION 5.2.3
	if((ptr = strchr(ligne, '/')) != NULL){
	  fprintf(fclient, message_bienvenue);
	}

	
	
	free(buffer);
      }

       

      close(socket_client);
      
      return socket_client;
      
    }
  }
  
}
