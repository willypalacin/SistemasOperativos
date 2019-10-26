#ifndef _inout_H
#define _inout_H

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <math.h>
#include <signal.h>


#define FILE_ERROR "No se ha encontrado el archivo\n"
#define STRING_1 "SHOW OPTIONS\n"
#define STRING_2_1 "CONNECT"
#define STRING_3 "SAY"

#define END_CHAR '\n'

typedef struct {
  char * username;
  char * audios;
  char * ip;
  char * port;
  char ** conex;
  char ** users;
  int q_conex;
  int q_users;
} User;



#endif

void INOUT_readFile(char * nombre, User * user);
int INOUT_eligeOpcion(User * user);
