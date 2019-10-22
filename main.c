/*
Lluís Camino Pérez login: lluis.Camino
Guillermo Palacín Gomez login: guillermo.palacin
*/


#include "inout.h"

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
  int flag = 0;
  inicializaUser(&user);
  if(argc != 2) {
    write(1, ARGUMENT_ERROR, strlen(ARGUMENT_ERROR));
  } else {
    INOUT_readFile(argv[1], &user);
  }
  do {
    flag = eligeOpcion();
  }while (flag == 0) {
    /* code */
  }

  return 0;
}
