#ifndef _inout_H
#define _inout_H

#include "structures.h"
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
#include <pthread.h>
#include <signal.h>
#include "conexion.h"





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
#define PORT "%d\n"
#define CONNECTING "Connecting...\n"
#define CONNECTED " connected: "
#define DISCONNECTING "Disconnecting Trinity...\n"
#define END_CHAR '\n'
#define NO_USERS_FOUNDED "No hay usuarios encontrados con ese username\n"
#define WRITE_END 1
#define READ_END 0




void INOUT_readFile(char * nombre, User * user);
int INOUT_eligeOpcion(User * user);

#endif
