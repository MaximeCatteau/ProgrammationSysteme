#include <stdio.h>
#include <string.h>
#include "socket.h"

int main (/*int argc,char ** argv*/void)
{
  printf("Creation du serveur\n");

  int serveur = creer_serveur(8080);

  while(serveur != -1){
    printf("Message\n");
  }
  
  return 0;
}
