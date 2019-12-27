#ifndef _conexion_H
#define _conexion_H


#include "structures.h"
#include "main.h"
#include "inout.h"


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
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <pthread.h>
#include <unistd.h>
#include <dirent.h>






int CONEXION_launch_server(int port, char *ip, int *socket_fd);
void  * CONEXION_server_run(void * server_socket);
void CONEXION_inicializaThread(int * server_socket);
int CONEXION_tryConnection(char *ip, int port);
void CONEXION_enviarTrama(int fd, Trama trama);
int recepcioTrama(int fd, Trama *trama);
char *get_message(int fd, char delimiter);
int ConexionModo2(int socket_conn, char * texto);
int ConexionModo1(int socket_conn, User * user);
int ConexionModo0(int socket_conn, User * user);
int ConexionModo6(int socket_conn, char * texto);
char * ConexionModo4(int socket_conn);

#endif
