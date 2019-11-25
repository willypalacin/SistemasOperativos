#include "conexion.h"

#define MSG_ERROR			"Error creando el socket.\n"

#define MSG_ERR_ARGS        "Numero de argumentos incorrecto.\n"
#define MSG_ERR_SOCKET      "Error durante la creacion del socket.\n"
#define MSG_ERR_BIND        "Error durante el bind del puerto.\n"
#define MSG_ERR_LAUNCH      "Error en la creacion del servidor.\n"
#define MSG_ERR_COM         "Error en la comunicacion. Enlace interrumpido con cliente.\n"
#define MSG_ERR_FILE		"Error con el fichero de las preguntas.\n"
#define LISTEN_BACKLOG		64
//#define PORT_OPEN        "El puerto %d esta abierto"

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

int fd_client;

int fd_tryToConnect;

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

int CONEXION_launch_server(int port, char *ip, int *socket_fd) {
    struct sockaddr_in s_addr;
    //char buffer[200];
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

void * CONEXION_server_run(void * server_socket) {
    struct sockaddr_in c_addr;
    socklen_t c_len = sizeof (c_addr);
    int * s_s = (int *) server_socket;

	   //write(1, MSG_WELCOME, sizeof(MSG_WELCOME));
     //write(1, MSG_SERVER_INIT, sizeof(MSG_SERVER_INIT));

    while (1) {

        //write(1, MSG_WAIT_CONN, sizeof(MSG_WAIT_CONN));
        fd_client = accept((*s_s), (void *) &c_addr, &c_len);
        write(1, MSG_NEW_CONN, sizeof(MSG_NEW_CONN));
        char* question = get_message(fd_client, '\n');
        write(1, question, strlen(question));

        //sleep(20);



        //sleep(20);
        close(fd_client);
    }

	//close(fd_client);
}



int CONEXION_tryConnection(char *ip, int port) {
  int aux = 0;
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
        char buff[128];
      //  sprintf(buff, PORT_NOT_OPEN, port);
      //  write(1, buff, sizeof(buff));
        //int bytes = sprintf(buff, "errno says: %s\n", strerror(errno)); // molt útil
        //write(1, buff, bytes);
			  close(socket_conn);
        socket_conn = -1;
      }
      else {
        char buff[128];
        //sprintf(buff, PORT_OPEN, port);
        //write(1, buff, sizeof(buff));
        //(*counter)++;
        char antonio[20] = "antonio\n";
        write(socket_conn, antonio, strlen(antonio));
        close(socket_conn);

      }
    }

    return socket_conn;
}



void CONEXION_inicializaThread(int * server_socket) {
  pthread_t t1;
  pthread_create (&t1, NULL, CONEXION_server_run, server_socket);
}
