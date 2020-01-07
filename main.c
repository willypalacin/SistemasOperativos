/*
Lluís Camino Pérez login: lluis.Camino
Guillermo Palacín Gomez login: guillermo.palacin
*/

#include "main.h"
#include "inout.h"
#include "structures.h"
#include "conexion.h"




#define ARGUMENT_ERROR "EL NUMERO DE ARGUMENTOS ES INCORRECTO\n"
#define ST "Starting Trinity...\n"
#define USERNAME "$%s:"
#define INCORRECT_FORMAT "La opcion no es correcta, asegurese de introducirlo segun las instrucciones\n"


int server_socket;
User user;
/*
Funcion que inicia los parametros necesarios para el usuario.
*/
void inicializaUser(User * user) {
  (*user).username = malloc(sizeof(char));
  (*user).audios = malloc(sizeof(char));
  (*user).ip = malloc(sizeof(char));
  (*user).port = malloc(sizeof(char));
  (*user).url = malloc(sizeof(char));
  //(*user).ports = malloc(sizeof(int));
  (*user).users = malloc(sizeof(char *));
  (*user).users[0] = malloc(sizeof(char)*50);

  (*user).q_ports = 0;
  (*user).ports_available = malloc(sizeof(int));
  (*user).port_asociated_user = malloc(sizeof(int));
  (*user).real_port_asociated_user = malloc(sizeof(int));

  (*user).users_del_server = malloc(sizeof(char *));
  (*user).users_del_server[0] = malloc(sizeof(char)*50);
  (*user).port_asociated_user_del_server = malloc(sizeof(int));
  (*user).q_users_del_server = 0;

}



void signalHandler(int sig) {

	switch (sig) {
		case SIGINT:
			write(1, DISCONNECTING, sizeof(DISCONNECTING));
      liberaMemoria(&user);
			raise(SIGKILL);
			break;
	}
}

User * MAIN_getUser() {
  return &user;
}

int main(int argc, char**argv) {

  int flag = 0;
  write(1,ST,strlen(ST));
  inicializaUser(&user);
  signal(SIGINT, signalHandler);
  if(argc != 2) {
    write(1, ARGUMENT_ERROR, strlen(ARGUMENT_ERROR));
  } else {
    INOUT_readFile(argv[1], &user);
    if (CONEXION_launch_server(atoi((user).port), (user).ip, &server_socket)) {
      //signal(SIGINT, handle_signal);
      CONEXION_inicializaThread(&server_socket);
    } else {
      exit(0);
    }
    flag++;
  }

  do {
    write(1,"$",1);
    write(1,user.username,strlen(user.username));
    write(1,":",1);

    flag = INOUT_eligeOpcion(&user);
    if(flag==0) {
      write(1, INCORRECT_FORMAT, strlen(INCORRECT_FORMAT));
    }

  } while (flag != 7);

  return 0;
}
