#ifndef _conexion_H
#define _conexion_H

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


typedef struct{
  char type;
  char * header;
  int longitud;
  char * data;
} Trama;

int CONEXION_launch_server(int port, char *ip, int *socket_fd);
void  * CONEXION_server_run(void * server_socket);
void CONEXION_inicializaThread(int * server_socket);
int CONEXION_tryConnection(char *ip, int port, char* username);
void enviarTrama(int fd, Trama trama);
void recepcioTrama(int fd, Trama *trama);
char *get_message(int fd, char delimiter);

#endif
