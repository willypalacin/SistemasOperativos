#include "conexion.h"


#define MSG_ERROR			"Error creando el socket.\n"

#define MSG_ERR_ARGS        "Numero de argumentos incorrecto.\n"
#define MSG_ERR_SOCKET      "Error durante la creacion del socket.\n"
#define MSG_ERR_BIND        "Error durante el bind del puerto.\n"
#define MSG_ERR_LAUNCH      "Error en la creacion del servidor.\n"
#define MSG_ERR_COM         "Error en la comunicacion. Enlace interrumpido con cliente.\n"
#define MSG_ERR_FILE		"Error con el fichero de las preguntas.\n"
#define LISTEN_BACKLOG		64
#define MSG_WAIT_CONN       "[Esperando conexiones]\n"
#define MSG_SERVER_INIT     "[Servidor en funcionamiento]\n"
#define MSG_NEW_CONN        "[Conexion establecida]\n"
#define MSG_CLOSE			"[Cerrando servidor]\n"
#define MSG_WELCOME			"Rosco de la Salle - Primera Edición\n"
#define MSG_WAIT_NAME		"Esperando a que el concursante introduzca su nombre...\n"
#define MSG_NEW_CONTESTANT	"Tenemos un nuevo concursante! %s, empieza tu rosco!\n"
#define MSG_QUESTION		"Pregunta %d enviada, esperando la respuesta de %s\n"
#define MSG_END				"Gracias por tu participación, %s. Tu puntuacion ha sido de %d\n"
#define MSG_DISCONNECT		"El concursante %s se ha desconectado de manera inesperada. Puntuación obtenida: %d\n"
#define PORT_NOT_OPEN   "El puerto %d no está abierto"
#define PORT_OPEN     "El puerto %d sí está abierto"
#define AUX "[%s]"
#define TR_NAME "[TR_NAME]"
#define CON_OK "[CON_OK]"
#define M_S_G "[MSG]"

int fd_client;
int fd_tryToConnect;


void CONEXION_enviarTrama(int fd, Trama trama){
	write (fd, &(trama.type), sizeof(char));
	char * infoHeader = malloc (sizeof(char) * strlen(trama.header));
	sprintf(infoHeader, AUX, trama.header);
	write (fd, infoHeader, strlen(infoHeader));
	write (fd, &(trama.longitud), sizeof(int));
	char * data = malloc (sizeof(char) * (strlen((trama.data)) + 2));
	sprintf(data, AUX, trama.data);
	write (fd, data, strlen(data));
	free(data);
	free (infoHeader);
}

char *get_message(int fd, char delimiter) {
	char *msg = (char *) malloc(1);
	char current;
	int i = 0;

	while (read(fd, &current, 1) > 0) {
		msg[i] = current;
		msg = (char *) realloc(msg, ++i + 1);
		if (current == delimiter)
			break;
	}

	msg[i] = '\0';

	return msg;
}

char * readUntillChar(char* s, char init ,char end){
  char * aux = malloc(sizeof(char)*200);
  aux[0]=' ';
  unsigned int i = 0;
  for(i = 0; i<strlen(s); i++) {
    if(s[i] == init) break;
  }
  i++;
  int j = 0;
  while (s[i] != end) {
    aux[j] = s[i];
    i++; j++;
    if(i>=strlen(s)) break;
  }
  if(i>strlen(s)) {
    aux[0]=' ';
    aux[1]='\0';
  }
  aux[i - 1] = '\0';
  return aux;
}

int recepcioTrama(int fd, Trama *trama){
	(*trama).type = -1;
	read(fd, &((*trama).type), sizeof(int));
	(*trama).header = get_message(fd, ']');
	read(fd, &((*trama).longitud), sizeof(int));
	(*trama).data = get_message(fd, ']');
	return ((*trama).type - '0');
}

int CONEXION_launch_server(int port, char *ip, int *socket_fd) {
    struct sockaddr_in s_addr;
    *socket_fd = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (*socket_fd < 0)
        write(1, MSG_ERR_SOCKET, sizeof(MSG_ERR_SOCKET));
    else {
        bzero (&s_addr, sizeof (s_addr));
        s_addr.sin_family = AF_INET;
        s_addr.sin_port = htons (port);
        s_addr.sin_addr.s_addr = inet_addr(ip);

        if (bind (*socket_fd, (void *) &s_addr, sizeof (s_addr)) < 0)
            write(1, MSG_ERR_BIND, sizeof(MSG_ERR_BIND));
        else {
         	listen(*socket_fd, LISTEN_BACKLOG);
          return 1;
        }
    }
    return 0;
}

void * CONEXION_gestionFdClientes(void * fd_c) {
	Trama trama;
	Trama trama2;
	char aux [200];
	User * user = MAIN_getUser();
	int t;
	char * usnm;
	int * fc = (int *) fd_c;
	int fd_cl = *fc;
	int index;
	while (1){
		t = recepcioTrama(fd_cl, &trama);
		switch (t) {
			case 0:
			 close(fd_cl);
			 break;
			case 1:
			  index = (*user).q_users_del_server;
				(*user).q_users_del_server = (*user).q_users_del_server + 1;
				(*user).users_del_server = realloc((*user).users_del_server, (*user).q_users_del_server + 1);
				(*user).port_asociated_user_del_server = realloc((*user).port_asociated_user_del_server, (*user).q_users_del_server + 1);
				strcpy((*user).users_del_server[(*user).q_users_del_server - 1], trama.data);
				(*user).port_asociated_user_del_server[(*user).q_users_del_server - 1] = fd_client;
				trama2.type = '1';
				trama2.header = malloc (sizeof(char) * strlen(CON_OK));
				strcpy(trama2.header, "CONOK");
				trama2.longitud = strlen((*user).username);
				trama2.data = malloc (sizeof(char) * trama2.longitud);
				strcpy(trama2.data, (*user).username);
				CONEXION_enviarTrama(fd_client, trama2);

				break;
			case 2:
			  write(1, "\n$", 3);
			  usnm = readUntillChar((*user).users_del_server[index],'[', ']');
				write(1, usnm, strlen(usnm));
				write(1, ": ", 2);
				char * data = readUntillChar(trama.data, '[', ']');
				write(1, data, strlen(data));
				break;
		}
		//close(fd_client);
	}
	return 0;
}



void * CONEXION_server_run(void * server_socket) {
    struct sockaddr_in c_addr;
    socklen_t c_len = sizeof (c_addr);
    int * s_s = (int *) server_socket;
		pthread_t thr[64];
		User * user = MAIN_getUser();
    while (1) {

     fd_client = accept((*s_s), (void *) &c_addr, &c_len);
		 pthread_create (&thr[(*user).q_users_del_server ], NULL, CONEXION_gestionFdClientes, &fd_client);


	 }
 }

int CONEXION_tryConnection(char *ip, int port) {
  struct sockaddr_in s_addr;
  int socket_conn = -1;
  socket_conn = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (socket_conn < 0)
      write(1, MSG_ERR_SOCKET, sizeof(MSG_ERR_SOCKET));
    else {
      memset(&s_addr, 0, sizeof(s_addr));
      s_addr.sin_family   = AF_INET;
      s_addr.sin_port     = htons(port);
      s_addr.sin_addr.s_addr = inet_addr(ip);
      if (connect(socket_conn, (void *) &s_addr, sizeof(s_addr)) < 0) {
			  close(socket_conn);
        socket_conn = -1;
      }

		}
    	return socket_conn;
	}

	/*Envia la trama correspondiente al modo 0*/
int ConexionModo0(int socket_conn, User * user) {
	Trama trama;
	trama.type = '0';
	trama.header = malloc (sizeof(char) * strlen(TR_NAME));
	strcpy(trama.header, "TR_NAME");
	trama.longitud = strlen((*user).username);
	trama.data = malloc (sizeof(char) * trama.longitud);
	strcpy(trama.data, (*user).username);
	CONEXION_enviarTrama(socket_conn, trama);
	close(socket_conn);
	return 0;
}

int ConexionModo1(int socket_conn, User * user){
		Trama trama;
		Trama trama2;
		int t2;
		trama.type = '1';
		trama.header = malloc (sizeof(char) * strlen(TR_NAME));
		strcpy(trama.header, "TR_NAME");
		trama.longitud = strlen((*user).username);
		trama.data = malloc (sizeof(char) * trama.longitud);
		strcpy(trama.data, (*user).username);
		CONEXION_enviarTrama(socket_conn, trama);
		t2 =  recepcioTrama(socket_conn, &trama2);
				//	if(t2 == 1) vamos a suponer que t2 == 1
		(*user).q_users = (*user).q_users + 1;
		(*user).users = realloc((*user).users, (*user).q_users + 1);
		(*user).port_asociated_user = realloc((*user).port_asociated_user, (*user).q_users + 1);
		strcpy((*user).users[(*user).q_users - 1], trama2.data);
		(*user).port_asociated_user[(*user).q_users - 1] = socket_conn;
		return 1;
}

int ConexionModo2(int socket_conn, User * user, char * texto) {
	Trama trama;
	//Trama trama2;

	trama.type = '2';
	trama.header = malloc (sizeof(char) * strlen(M_S_G));
	strcpy(trama.header, "MSG");
	trama.longitud = strlen(texto) + 2;
	trama.data = malloc (sizeof(char) * trama.longitud);
	strcpy(trama.data, texto);
	CONEXION_enviarTrama(socket_conn, trama);
	return 2;
}



void CONEXION_inicializaThread(int * server_socket) {
  pthread_t t1;
  pthread_create (&t1, NULL, CONEXION_server_run, server_socket);
}
