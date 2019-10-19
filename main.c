/*
Lluís Camino Pérez login: lluis.Camino
Guillermo Palacín Gomez login: guillermo.palacin
*/


#include "ficheros.h"

#define ARGUMENT_ERROR "EL NUMERO DE ARGUMENTOS ES INCORRECTO\n"



void inicializaUser(User * user) {
  (*user).username = malloc(sizeof(char));
  (*user).audios = malloc(sizeof(char));
  (*user).ip = malloc(sizeof(char));
  (*user).port = malloc(sizeof(char));
  (*user).conex = malloc(sizeof(char *));
  (*user).conex[0] = malloc(sizeof(char));
}

int main(int argc, char**argv) {
  User user;
  inicializaUser(&user);
  if(argc != 2) {
    write(1, ARGUMENT_ERROR, strlen(ARGUMENT_ERROR));
  } else {
    FICHEROS_readFile(argv[1], &user);
  }
  return 0;
}
