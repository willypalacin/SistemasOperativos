#ifndef _FICHEROS_H
#define _FICHEROS_H

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
#define END_CHAR '\n'

typedef struct {
  char * username;
  char * audios;
  char * ip;
  char * port;
  char ** conex;
} User;



#endif

void FICHEROS_readFile(char * nombre, User * user);
