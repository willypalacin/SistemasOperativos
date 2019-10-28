#ifndef _inout_H
#define _inout_H

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <ctype.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <math.h>
#include <signal.h>


#define FILE_ERROR "No se ha encontrado el archivo\n"
#define STRING_1 "SHOW CONNECTIONS"
#define STRING_2_1 "CONNECT"
#define STRING_3 "SAY"
#define STRING_4 "BROADCAST"
#define STRING_5 "SHOW AUDIOS"
#define STRING_6 "DOWNLOAD"
#define STRING_7 "EXIT"
#define TESTING "Testing...\n"
#define CONEX_AVAIL "%d connections available\n"
#define CONNECTING "Connecting...\n"
#define CONNECTED " connected: "
#define DISCONNECTING "Disconnecting Trinity...\n"

#define END_CHAR '\n'

typedef struct {
  char * username;
  char * audios;
  char * ip;
  char * port;
  char * url;
  int  * ports;
  char ** users;
  int q_ports;
  int q_users;
} User;



#endif

void INOUT_readFile(char * nombre, User * user);
int INOUT_eligeOpcion(User * user);
