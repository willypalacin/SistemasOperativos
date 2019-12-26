#ifndef _structures_H
#define _structures_H

typedef struct {
  char * username;
  char * audios;
  char * ip;
  char * port;
  char * url;
  int  * ports;
  int * ports_available;
  int q_ports_available;
  char ** users;
  int * port_asociated_user;
  int q_ports;
  int q_users;
  int server_socket;
  char mensaje[180];
  char ** users_del_server;
  int * port_asociated_user_del_server;
  int q_users_del_server;
} User;

typedef struct {
  char type;
  char * header;
  int longitud;
  char * data;
} Trama;

#endif
