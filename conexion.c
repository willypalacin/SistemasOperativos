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

#define PORT_NOT_OPEN   "El puerto %d no está abierto"
#define PORT_OPEN     "El puerto %d sí está abierto"
#define AUX "[%s]"
#define TR_NAME "[TR_NAME]"
#define CON_OK "[CON_OK]"
#define M_S_G "[MSG]"
#define CONKO "[CONKO]"
#define SH_AUDIOS "[SHOW_AUDIOS]"
#define BROADCAST "[BROADCAST]"
#define USER_CONNECTED "Usuario conectado: %s\n"

int fd_client;
int fd_tryToConnect;
static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

char * ejecutaMD5(char * nombre) {
  pid_t pid;
  int fd[2];
  pipe(fd);
  pid = fork();

  char * buffer = malloc(sizeof(char)*1000);
  if(pid==0){
      dup2(fd[WRITE_END], 1); //duplicamos el fd de escribir
      close(fd[READ_END]);
      //execl("md5sum",nombre, (char*) NULL);
      execlp("md5sum" , "md5sum", nombre ,(char*) NULL);
      write (1, "Failed to execute show_connections.sh\n", strlen("Failed to execute show_connections.sh\n"));
      close(fd[WRITE_END]);
      exit(1);
  }else{
      close(fd[WRITE_END]);
      wait(&pid);
      read(fd[READ_END], buffer, sizeof(char)*1000);
      //write(1, buffer, strlen(buffer));
      close(fd[READ_END]);
  }
  return buffer;
}

char * readFile(char * path, int * longitud){
  int fd,retval,fd2;
  char * buffer = malloc(sizeof(char));
  char * buffer2 = malloc(sizeof(char));
  char path2[128];

  /*opening the file in read-only mode*/
  if ((fd = open(path, O_RDONLY)) < 0) {
      perror("Problem in opening the file");
			return "ERROR";
      exit(1);
  }

  /*reading bytes from the fd and writing it to the buffer*/
  int i = 0;
  while ((retval = read(fd, buffer, 1)) > 0){
      buffer2= realloc(buffer2, i+2);
      buffer2[i] = *buffer;
      i++;
  }
  *longitud = i;
  if (retval < 0) {
      perror("\nRead failure");
      exit(1);
  }

  close(fd);


  return buffer2;
}

int openFile(char * path) {
  int fd;
  if ((fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0640)) < 0) {
      perror("Problem in opening the file");
      return -1;
      exit(1);
  }
  return fd;

}
/*Funcion que escribe el archivo de audio*/
int writeToFile(char * path, int longitud, char * data){
  int fd,retval;
  int i;

  /*opening the file in read-only mode*/
  if ((fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0640)) < 0) {
      perror("Problem in opening the file");
			return -1;
      exit(1);
  }
  for (i=0;i<longitud;i++) {
    write(fd, &data[i], 1);
  }
  close(fd);

  return fd;
}


void CONEXION_enviarTrama(int fd, Trama trama){
  char * data = malloc (sizeof(char) * 500);
	sprintf(data, AUX, trama.data);

  write (fd, &(trama.type), sizeof(char));
  printf("LONG TRANSMITIDA -%d- \n", trama.longitud );
  printf("TRAMA TRANSMITIDA -%s- \n", trama.header );
	write (fd, trama.header, strlen(trama.header));
	write (fd, &(trama.longitud), sizeof(short));
	//if(trama.longitud!=0) {
  printf("DATA TRANSMITIDA -%s- \n", data );
	write (fd, data, strlen(data));
	free(data);
	//}
}

void CONEXION_enviarTramaAudio(int fd, Trama trama){
  int i;
  write (fd, &(trama.type), sizeof(char));
  printf("LONG TRANSMITIDA ,%d,\n", trama.longitud );
  printf("HEADER TRANSMITIDA ,%s, \n", trama.header);
	write (fd, trama.header, strlen(trama.header));
	write (fd, &(trama.longitud), sizeof(short));
	//if(trama.longitud!=0) {
  printf("DATA TRANSMITIDA:\n");
  for(i = 0; i< trama.longitud; i++) {
    write(1, &trama.data[i], sizeof(char));
  }
	write (fd, trama.data, trama.longitud);
	//}
}
/*
Funcion que se encarga de leer los archivos pasado por parametro y los devuelve.
*/
char * leeDirectorio(char * directorio) {
  struct dirent *de;
  char buff[100];

  char * audiosFiles = malloc(sizeof(char)*500);
  sprintf(buff, "./%s", directorio);
  DIR *dr = opendir(buff);
  if (dr == NULL)  {
    printf("No se ha podido abrir el directorio");
    return 0;
  }
  while ((de = readdir(dr)) != NULL){
        if(strcmp(de->d_name, ".")!= 0 && strcmp(de->d_name, "..")!=0){
          strcat(audiosFiles,de->d_name);
          strcat(audiosFiles,"\n");
        }
      }

  closedir(dr);
  return audiosFiles;
}

char * get_message(int fd, char delimiter) {
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

char * get_message_longitud(int fd, int longitud) {
	char *msg = (char *) malloc(1);
	char current;
	int i = 0;

	while (read(fd, &current, 1) > 0) {
		msg[i] = current;
		msg = (char *) realloc(msg, ++i + 1);
		if (i == longitud)
			break;
	}

	msg[i] = '\0';

	return msg;
}

char * readUntillChar(char* s, char init ,char end){
  char * aux = malloc(sizeof(char)*500);
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
	read(fd, &((*trama).type), sizeof(char));
	(*trama).header = get_message(fd, ']');
	read(fd, &((*trama).longitud), sizeof(short));
	//if ((*trama).longitud == 0) return ((*trama).type - '0');
	(*trama).data = get_message(fd, ']');
	return ((*trama).type - '0');
}

Trama recepcioTramaAudio(int fd, int fdFile){
  Trama trama;
	(trama).type = -1;
	read(fd, &((trama).type), sizeof(char));
	(trama).header = get_message(fd, ']');
	read(fd, &((trama).longitud), sizeof(short));
	//if ((*trama).longitud == 0) return ((*trama).type - '0');
  trama.data = malloc(trama.longitud);
  read(fd, trama.data, trama.longitud);
  if(strcmp(trama.header, "[EOF]")!=0)
    write(fdFile, trama.data, trama.longitud);

	return trama;
}

Trama CONEXION_creaTrama(char type, char header[50], short longitud, char * data) {
  Trama trama;
  trama.data = malloc((sizeof(char) * longitud));
  trama.data = data;
  trama.type = type;
  trama.header = malloc(strlen(header));
  strcpy(trama.header,header);
  trama.longitud = longitud;
  return trama;
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

        if (bind (*socket_fd, (void *) &s_addr, sizeof (s_addr)) < 0) {
            write(1, MSG_ERR_BIND, sizeof(MSG_ERR_BIND));
            return 0;
        }
        else {
         	listen(*socket_fd, LISTEN_BACKLOG);
          return 1;
        }
    }
    return 0;
}

Trama getTrama() {
  Trama trama;
  return trama;
}

int findClientAndDelete(char * usnm, int * port_delete, char ** users, int q_users){
	int i;
	int index = -1;
	for(i=0;i<q_users;i++){
		if (strcmp(usnm, users[i]) == 0) {
			index = i;
			users[i][0] = '\0';
			port_delete[i] = 0;
			return i;
		}
	}
	return index;

}

void CONEXION_mutexDestroy(){
  pthread_mutex_destroy(&mtx);
}

void CONEXION_receiveBroadcast(int socket_conn, char * name){
  Trama trama2;
  int s;
  recepcioTrama(socket_conn, &trama2);
  pthread_mutex_lock(&mtx);
  write(1, "\n[", 2);
  write(1, name, strlen(name));
  write(1, "] ", 2);
  write(1, "Cool!", strlen("Cool!"));
  pthread_mutex_unlock(&mtx);
}



void * CONEXION_gestionFdClientes(void * fd_c) {
	Trama trama;
	Trama trama2;
  Trama * trama3 = malloc(sizeof(Trama));
	char * aux = malloc(sizeof(char)*500);
	char * buff3 = malloc(sizeof(char)*128);
	char * fileAudio;
  char * buff = malloc(sizeof(char)*128);
	char * buff2 = malloc(sizeof(char)*128);
	User * user = MAIN_getUser();
	int t;
	unsigned int sizeRead, leido ,w, longitud;
	sizeRead = 0;
	char * md5Audio;
  int count;
	char * usnm;
	int * fc = (int *) fd_c;
  int fdFile;
  int fd2;
  char path2[100];
	int fd_cl = *fc;
	int index;
  longitud = 0;
  write(1, "HOLA2", 5);
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
        (*user).users_del_server[(*user).q_users_del_server - 1] = malloc(sizeof(char)*100);
				strcpy((*user).users_del_server[(*user).q_users_del_server - 1], trama.data);
				(*user).port_asociated_user_del_server[(*user).q_users_del_server - 1] = fd_cl;
        sprintf(buff, USER_CONNECTED, readUntillChar(trama.data, '[' ,']'));
        write(1, buff, strlen(buff));
				trama2.type = '1';
				trama2.header = malloc (sizeof(char) * strlen(CON_OK));
				strcpy(trama2.header, CON_OK);
				trama2.longitud = strlen((*user).username);
				trama2.data = malloc (sizeof(char) * trama2.longitud);
				strcpy(trama2.data, (*user).username);
				CONEXION_enviarTrama(fd_cl, trama2);
        write(1,"$",2);
        write(1,(*user).username,strlen((*user).username));
        write(1,":",1);


				break;
			case 2:
			  write(1, "\n[", 2);
			  usnm = readUntillChar((*user).users_del_server[index],'[', ']');
				write(1, usnm, strlen(usnm));
				write(1, "]: ", 3);
				char * data = readUntillChar(trama.data, '[', ']');
				write(1, data, strlen(data));
				trama2.type = '2';
				trama2.header = malloc (sizeof(char) * strlen("[MSGOK]"));
				strcpy(trama2.header, "[MSGOK]");
				trama2.longitud = 0;
				CONEXION_enviarTrama(fd_cl, trama2);
        write(1,"\n$",2);
        write(1,(*user).username,strlen((*user).username));
        write(1,":",1);
				break;

      case 3:
        write(1, "\n[", 2);
        usnm = readUntillChar((*user).users_del_server[index],'[', ']');
        write(1, usnm, strlen(usnm));
				write(1, "]: ", 3);
				char * data1 = readUntillChar(trama.data, '[', ']');
				write(1, data1, strlen(data));
        trama2.type = 3;
        trama2.header = malloc (sizeof(char) * strlen("[MSGOK]"));
        strcpy(trama2.header, "[MSGOK]");
        trama2.longitud = 0;
        CONEXION_enviarTrama(fd_cl, trama2);
        write(1,"\n$",2);
        write(1,(*user).username,strlen((*user).username));
        write(1,":",1);
        break;

			case 4:
				trama2.type = '4';
				trama2.data = leeDirectorio((*user).audios);
				trama2.header = malloc (sizeof(char) * strlen("[LIST_AUDIOS]"));
				strcpy(trama2.header, "[LIST_AUDIOS]");
				trama2.longitud = 0;
				CONEXION_enviarTrama(fd_cl, trama2);
				break;
			case 5:

			  trama2.type = '5';
				sprintf(buff3, "./%s/%s", (*user).audios, readUntillChar(trama.data, '[', ']'));
		   	fileAudio = readFile(buff3, &longitud);

        sprintf(path2, "./prueba/a.mp3");
        if ((fd2 = open(path2, O_WRONLY | O_CREAT | O_TRUNC, 0640)) < 0) {
            perror("Problem in opening the file");
      			return -1;
            exit(1);
        }

				if(strcmp(fileAudio, "ERROR")==0) {
					trama2.header = malloc (sizeof(char) * strlen("[AUDIO_KO]"));
					strcpy(trama2.header, "[AUDIO_KO]");
					trama2.longitud = 0;
					strcpy(trama2.data, "");
				}else {
          count = 0;
          md5Audio = ejecutaMD5(buff3);
          printf("longitud: -%d- \n", longitud );
          for(w = 0; w < longitud; w++) {
            aux[count] = fileAudio[w];
            count++;
            if(count == 500 && w!=0){
              trama2 = CONEXION_creaTrama('5', "[AUDIO_RSPNS]", count, aux);
              CONEXION_enviarTramaAudio(fd_cl, trama2);
              write(fd2, trama2.data, trama2.longitud);

              printf("w: -%d- \n", w );
              count = 0;
              free(aux);
              aux = malloc(sizeof(char) * 500);

            }


            if(w >= longitud) break;
          }
          trama2 = CONEXION_creaTrama('5', "[AUDIO_RSPNS]", count, aux);
          write(fd2, trama2.data, trama2.longitud);
          CONEXION_enviarTramaAudio(fd_cl, trama2);
          close(fd2);
          trama2 = CONEXION_creaTrama('5', "[EOF]", strlen(md5Audio), md5Audio);
          CONEXION_enviarTramaAudio(fd_cl, trama2);
				}
				break;
			case 6:
				trama2.type = '6';
				trama2.header = malloc (sizeof(char) * strlen(CONKO));
				strcpy(trama2.header, "[CONKO]");
				trama2.longitud = 0;
				trama2.data = "";
				CONEXION_enviarTrama(fd_cl, trama2);
				write(1, trama.data, strlen(trama.data));
				sprintf(buff2, "El cliente %s se ha desconectado\n",  readUntillChar(trama.data, '[', ']'));
				findClientAndDelete(trama.data, (*user).real_port_asociated_user, (*user).users, (*user).q_users);
				write(1, buff2, strlen(buff2));
				close(fd_cl);
        break;
      //default:
        //close(fd_cl);

		}

		//close(fd_client);

	}
	return 0;
}



void * CONEXION_server_run(void * server_socket) {
    struct sockaddr_in c_addr;
    socklen_t c_len = sizeof (c_addr);
    int i = 0;
    char c;
    char buff[20];
    int * s_s = (int *) server_socket;
		pthread_t thr[64];
		User * user = MAIN_getUser();
    (*user).server_socket = malloc(sizeof(int));

    while (1) {
     char fd;
     fd_client = accept((*s_s), (void *) &c_addr, &c_len);
     printf("Hay accept");
     fd = '0' + fd_client;
     write(1, &fd, 1);
		 pthread_create (&thr[(*user).q_users_del_server ], NULL, CONEXION_gestionFdClientes, &fd_client);
     //(*user).server_socket = realloc((*user).server_socket, i+2);
     //(*user).server_socket[i] = fd_client;
     i++;
     write(1, "HOLA1", 5);
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
	strcpy(trama.header, "[TR_NAME]");
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
		trama.type = '1';
		trama.header = malloc (sizeof(char) * strlen(TR_NAME));
		strcpy(trama.header, "[TR_NAME]");
		trama.longitud = strlen((*user).username);
		trama.data = malloc (sizeof(char) * trama.longitud);
		strcpy(trama.data, (*user).username);
		CONEXION_enviarTrama(socket_conn, trama);
		recepcioTrama(socket_conn, &trama2);
				//	if(t2 == 1) vamos a suponer que t2 == 1

		(*user).q_users = (*user).q_users + 1;
		(*user).users = realloc((*user).users, (*user).q_users + 1);
    (*user).users[(*user).q_users - 1] = malloc(sizeof(char) * 100);
		(*user).port_asociated_user = realloc((*user).port_asociated_user, (*user).q_users + 1);
    (*user).real_port_asociated_user = realloc((*user).real_port_asociated_user, (*user).q_users + 1);
		strcpy((*user).users[(*user).q_users - 1], trama2.data);
		(*user).port_asociated_user[(*user).q_users - 1] = socket_conn;
		return 1;
}

int ConexionModo2(int socket_conn, char * texto) {
	Trama trama;
	Trama trama2;
	trama.type = '2';
	trama.header = malloc (sizeof(char) * strlen(M_S_G));
	strcpy(trama.header, "[MSG]");
	trama.longitud = strlen(texto) + 2;
	trama.data = malloc (sizeof(char) * trama.longitud);
	strcpy(trama.data, texto);
	CONEXION_enviarTrama(socket_conn, trama);
	recepcioTrama(socket_conn, &trama2);
	return 2;
}

int ConexionModo3(int socket_conn, char * texto){
  Trama trama;
  Trama trama2;
  trama.type = '3';
  trama.header = malloc (sizeof(char) * strlen(BROADCAST));
  strcpy(trama.header, "[BROADCAST]");
  trama.longitud = strlen(texto) + 2;
  trama.data = malloc(sizeof(char) * trama.longitud);
  strcpy(trama.data, texto);
  CONEXION_enviarTrama(socket_conn, trama);
  return 3;
}

char * ConexionModo4(int socket_conn) {
	Trama trama;
	Trama trama2;
	trama.type = '4';
	trama.header = malloc(sizeof(char) * strlen(SH_AUDIOS));
	strcpy(trama.header, "[SHOW_AUDIOS]");
	trama.longitud = 0;
	trama.data = "";
	CONEXION_enviarTrama(socket_conn, trama);
	recepcioTrama(socket_conn, &trama2);
	return readUntillChar(trama2.data, '[', ']');
}


char * readDataXchars(char * data, short longitud) {
  char * buffer = malloc(sizeof(char) * longitud);
  int i;
  for(i = 1; i < (longitud+1); i++) {
    buffer[i-1]= data[i];
  }
  return buffer;
}

void escribeEnArchivo(int fd, int longitud, char * data) {
  int i;
  for(i = 0; i<longitud; i++){
    write(fd, &data[i], 1);
  }
}
void  copyDataToAcum(char * acum, int longitud, char * data, short init) {
  int i;
  for (i=0;i<init;i++){
    acum[longitud+i] = data[i];
  }
}
int ConexionModo5(int socket_conn, char * texto, char * audios) {
  Trama trama;
  Trama trama2;
  int longitud = 0;
  char buff[128];
  char * acum = malloc(sizeof(char));
  sprintf(buff, "./%s/%s", audios, texto);
  int fdFile = openFile(buff);
  trama = CONEXION_creaTrama('5', "[AUDIO_RQST]", strlen(texto), texto);
  CONEXION_enviarTrama(socket_conn, trama);
  while(1) {
    trama2 = recepcioTramaAudio(socket_conn, fdFile);
    printf("Long %d\n", trama2.longitud);
    printf("Header %s\n", trama2.header );
    printf("Data %s\n",  trama2.data);
    free(trama2.data);

    if(strcmp(trama2.header, "[EOF]")==0){
      printf("LONF TOTAL TACHAN %d\n", longitud );
      close(fdFile);
      //writeToFile(buff, longitud, acum);

      break;
    }

    acum = realloc(acum, longitud + trama2.longitud);
    copyDataToAcum(acum, longitud, trama2.data, trama2.longitud);
    longitud = longitud + trama2.longitud;

  }
}

int ConexionModo6(int socket_conn, char * texto) {
	Trama trama;
	Trama trama2;
	trama.type = '6';
	trama.header = malloc (sizeof(char) * 2);
	strcpy(trama.header, "");
	trama.longitud = strlen(texto) + 2;
	trama.data = malloc (sizeof(char) * trama.longitud);
	strcpy(trama.data, texto);
	CONEXION_enviarTrama(socket_conn, trama);
	recepcioTrama(socket_conn, &trama2);
	if (trama2.type=='6') return 6;
	return 2;
}
void CONEXION_inicializaThread(int * server_socket) {
  pthread_t t1;
  pthread_create (&t1, NULL, CONEXION_server_run, server_socket);
}
