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

 if(serveur != -1){
   sleep(1);
   printf("Le serveur a ete cree !\n");
  }

 start(serveur);

  return 0;
}
