/*
Lluís Camino Pérez login: lluis.Camino
Guillermo Palacín Gomez login: guillermo.palacin
*/


#include "inout.h"


#define ARGUMENT_ERROR "EL NUMERO DE ARGUMENTOS ES INCORRECTO\n"
#define ST "Starting Trinity...\n"
#define USERNAME "$%s:"
#define INCORRECT_FORMAT "La opcion no es correcta, asegurese de introducirlo segun las instrucciones\n"

void inicializaUser(User * user) {
  (*user).username = malloc(sizeof(char));
  (*user).audios = malloc(sizeof(char));
  (*user).ip = malloc(sizeof(char));
  (*user).port = malloc(sizeof(char));
  (*user).url = malloc(sizeof(char));
  //(*user).ports = malloc(sizeof(int));
  (*user).users = malloc(sizeof(char *));
  (*user).users[0] = malloc(sizeof(char)*50);
  strcpy((*user).users[0], "antonio");
  (*user).q_ports = 0;

}

int main(int argc, char**argv) {
  User user;
  write(1,ST,strlen(ST));
  int flag = 0;
  inicializaUser(&user);
  if(argc != 2) {
    write(1, ARGUMENT_ERROR, strlen(ARGUMENT_ERROR));
  } else {
    INOUT_readFile(argv[1], &user);
  }
  do {
    write(1,"$",1);
    write(1,user.username,strlen(user.username));
    write(1,":",1);

    flag = INOUT_eligeOpcion(&user);
    if(flag==0) {
      write(1, INCORRECT_FORMAT, strlen(INCORRECT_FORMAT));
    }

  }while (flag != 7);

  return 0;
}
