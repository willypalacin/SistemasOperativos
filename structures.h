#ifndef _structures_H
#define _structures_H

typedef struct {
  //--------------------
  //Parametros de la lectura del fichero.
  char * username;
  char * audios;
  char * ip;
  char * port;
  char * url;
  int  * ports;
  //---------------------
  //Lectura de puertos disponibles en show connections.
  int * ports_available;
  int q_ports_available;
  //--------------------
  //Estructura del cliente
  char ** users;
  int * port_asociated_user;
  int * real_port_asociated_user;//Files Descriptors.
  int q_ports;
  int q_users;
  //---------------
  //Estructura del servidor
  int * server_socket;
  char mensaje[180];
  char ** users_del_server;
  int * port_asociated_user_del_server;
  int q_users_del_server;
  //----------------------
} User;

typedef struct {
  char type;
  char * header;
  short longitud;
  char * data;
} Trama;

#endif
