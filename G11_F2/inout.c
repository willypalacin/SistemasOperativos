#include "inout.h"


/*
This function reads until arrives to the end character.
The function returns the number of bytes readed.
*/

char * readShowCon(User * user) {
  char port1[6];
  char port2[6];
  int q_ports = (*user).q_ports;
  sprintf (port1, "%d", (*user).ports[0]);
  sprintf (port2, "%d", (*user).ports[q_ports-1]);
  pid_t pid;
  int fd[2];
  pipe(fd);
  pid = fork();
  char * buffer = malloc(sizeof(char)*1000);

  if(pid==0){
      dup2(fd[WRITE_END], 1); //duplicamos el fd de escribir
      close(fd[READ_END]);
      execlp("bash", "show_connections_v2.sh" , "show_connections_v2.sh", port1, port2, "127.0.0.1" ,(char*) NULL);
      write (1, "Failed to execute show_connections.sh\n", strlen("Failed to execute show_connections.sh\n"));
      close(fd[WRITE_END]);
      exit(1);
  }else{
      close(fd[WRITE_END]);
      read(fd[READ_END], buffer, sizeof(char)*1000);
      //write(1, buffer, strlen(buffer));
      close(fd[READ_END]);
  }
  return buffer;
}


char * readUntil(int fd, char end, char * string) {
  ssize_t n;
  int i = 0;
  char c = '\0';
  while (c != end) {
    n = read(fd, &c, sizeof(char));
    if(n == 0) {
      return 0;
    }

    if (c != end) {
      string = (char*)realloc(string, sizeof(char) * (i + 2));
      string[i] = c;
    }
    i++;
  }
  string[i - 1] = '\0';
  return string;
}

/*
Version inspirada en la funcion readUntil
pero cuando no haya file descriptor
Se da por hecho que el caracter siempre va a existir
*/
char * readTillChar(char* s, char init ,char end){
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
/*
Busca si el usuario existe en el array y en caso armativo devuelve el puerto
*/
int findUserInArray(char * nom_user, User user) {
  int i;
  int port = -1;
  for(i=0;i<user.q_users;i++) {
    if(strcasecmp(user.users[i], nom_user) == 0)
      break;
  }
  port = user.port_asociated_user[i];
  return port;

}

char * readTillCharDouble(char* s, char init ,char end, int pos){
  char * aux = malloc(sizeof(char)*200);
  int counter = 0;
  aux[0]=' ';
  unsigned int i = 0;
  for(i = 0; i<strlen(s) && counter < pos; i++) {
    if(s[i]==init) counter++;
    if(s[i] == init && counter == pos) break;
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

void INOUT_readFile(char * nombre, User * user) {
  //ssize_t n;
  int i = 0;
  int j = 0;
  char * port_i = malloc(sizeof(char));
  char * port_f = malloc(sizeof(char));
  //int j;   //Number of ports
  int fd = open(nombre, O_RDONLY);
  if (fd < 0) {
    write(1, FILE_ERROR, strlen(FILE_ERROR));
  }
  else {
     readUntil(fd, END_CHAR, (*user).username);
     readUntil(fd, END_CHAR, (*user).audios);
     readUntil(fd, END_CHAR, (*user).ip);
     readUntil(fd, END_CHAR, (*user).port);
     (*user).url = readUntil(fd, END_CHAR, (*user).url);
     readUntil(fd, END_CHAR, port_i);
     readUntil(fd, END_CHAR, port_f);
    j = 0;
    (*user).ports = malloc(sizeof(int)*(atoi(port_f)-atoi(port_i)+2));
    for (i=atoi(port_i); i<=atoi(port_f);i++) {
      (*user).ports[j] = i;
      j++;
    }
    (*user).q_ports = atoi(port_f)-atoi(port_i)+1;

    /*while (n != 0) {
      n = readUntil(fd, END_CHAR, (*user).ports[i]);
      if (n == 0) break;
      (*user).ports = realloc ((*user).ports, sizeof(char *) * (i+2));
      (*user).ports[i+1] = malloc(sizeof(char));
      i++;
    }*/

  }

}
/*
Controla si los strings parseados son correctos.
Si es correcto devuelve la opciones.
Si no devuelve cero para volver al bucle principal.
*/
int controlError(char * a1, char * a2, int opt) {
  if(strcmp(a1," ") == 0|| strcmp(a2," ") == 0) {
    return 0;
  }
  return opt;
}
/*
Comprueba todas las posibles puertos disponibles y
si coincide con CONNECT <puertoExistente> sera correcto
*/
int checkStringCase2(User * user, char * s) {
  char aux[100];
  int  i;
  for (i = 0; i < (*user).q_ports_available; i++) {
   sprintf(aux, "CONNECT %d", (*user).ports_available[i]);
   if (strcmp(aux, s) == 0) {
     return (*user).ports_available[i];
   }
 }
  return -1;
}

void liberaMemoria(User * user,char * s) {
  int i;
  free((*user).username);
  free((*user).audios);
  free((*user).ip);
  free((*user).port);
  free((*user).url);
  free((*user).users_del_server);

  free(s);
  //Cierro Sockets
  for (i = 0; i< (*user).q_users_del_server; i++) {
    close((*user).port_asociated_user_del_server[i]);
  }
  free((*user).port_asociated_user_del_server);
}

char * substring(char * s, unsigned int init , unsigned int end) {
  unsigned int i;
  int j = 0;
  char * aux = malloc(sizeof(char)*50);
  if (strlen(s)>=end) {
    for (i = init; i < end; i++) {
      aux[j] = s[i];
      j++;
    }
    return aux;
  }
  else {
    return " ";
  }
}

void parseaPuertos(char * s, User * user){
  unsigned int i;
  unsigned int counter = 0;
  char * texto;
  char * puerto;
  char buff[128];
  char buff2[128];
  for (i=0;i<strlen(s); i++){
    if(s[i] == '\n') {
      counter++;
    }

  }
  (*user).q_ports_available = counter;
  (*user).ports_available = malloc(sizeof(int) * counter);
  sprintf(buff, CONEX_AVAIL, counter);
  write(1, buff, strlen(buff));
  for(i=0;i<counter; i++) {
    texto = readTillChar(s,'p','n');
    puerto = readTillChar(texto, ' ', ' ');
    (*user).ports_available[i] = atoi(puerto);
    sprintf(buff2, PORT, (*user).ports_available[i]);
    write(1, buff2, strlen(buff2));
  }


}

void messOpt1(User * user) {
  int i;
  int fd;
  int counter = 0;
  char buff[128];
  char buff2[128];
  (*user).q_ports_available = 0;
  free((*user).ports_available);
  (*user).ports_available = malloc(sizeof(int));

  for (i = 0; i < (*user).q_ports; i++) {
    fd = CONEXION_tryConnection((*user).ip, (*user).ports[i]);
    if (fd != -1) {
      ConexionModo0(fd, user);
      (*user).ports_available[counter] = (*user).ports[i];
      (*user).q_ports_available++;
      (*user).ports_available = realloc((*user).ports_available, counter +2);
      counter++;
    }
  }
  sprintf(buff, CONEX_AVAIL, counter);
  write(1, buff, strlen(buff));
  for(i = 0; i< counter; i++) {
    sprintf(buff2, PORT, (*user).ports_available[i]);
    write(1, buff2, strlen(buff2));

  }


  /*char *message = malloc(sizeof(char)* 70);
  char port_i[5];
  sprintf(port_i, "%d",(user).ports[0]);
  write(1, TESTING, strlen(TESTING));

  sprintf(message, CONEX_AVAIL, 1);
  write(1, message, strlen(message));
  write (1, port_i, strlen(port_i));
  write(1, "\n", 1);*/
}
/*
Funcion que hace de menu. Comprueba las diferentes opciones.
*/
int checkString(User * user, char * s) {

  int opcion;
  char * nom_user;
  //char * aux;
  char * texto;
  char port [6];
  char * audio;
  int t2;
  //char * message;

  if (strcmp(s, STRING_1) == 0) {
    //messOpt1(user);
    char * mensaje = readShowCon(user);
    //sleep(5);

    parseaPuertos(mensaje, user);
    return 1;
  }
  else if (strcmp(substring(s, 0, 7), STRING_2_1) == 0) {      //Comprueba si hay escrita la palabra CONNECT.
    opcion = checkStringCase2(user, s);  //Pone opcion pero devuelve el puerto
    if (opcion !=-1) {
      write(1, CONNECTING, strlen(CONNECTING));
      t2 = CONEXION_tryConnection((*user).ip, opcion);
      ConexionModo1(t2, user);
      if (t2 > 0){
        sprintf(port, "%d", opcion);
        write(1, port, strlen(port));
        write(1, CONNECTED, strlen(CONNECTED));
        int q = (*user).q_users;
        (*user).users[q-1] = substring((*user).users[q-1], 1, strlen((*user).users[q-1])-1);
        write(1, (*user).users[q-1], strlen((*user).users[q-1]));
        write(1, "\n", 1);
      }
      return 2;
    } else {
      return 0;
    }

  }
  else if(strcmp(substring(s, 0, 3), STRING_3) == 0) {
     int error;
     int port; //Comprueba si hay say
     nom_user = readTillChar(s,' ',' ');
     texto = readTillChar(s,'\"','\"');
     error = controlError(texto, nom_user, 3);
     port = findUserInArray(nom_user, *user); //En realidad port es fd.

     if(port!=-1) {
       //strcpy((*user).mensaje, texto);

       ConexionModo2(port, texto);

       //CONEXION_tryConnection((*user).ip, port, user, 2);
     }else {
       write(1, NO_USERS_FOUNDED, strlen(NO_USERS_FOUNDED));
     }
     return error;
  }
  else if(strcmp(substring(s, 0, 9), STRING_4) == 0) { //Comprueba si hay say
     int error;
     texto = readTillChar(s,'\"','\"');
     error = controlError(texto, "a", 4);

     return error;
  }
  else if(strcmp(substring(s, 0, 11), STRING_5) == 0) { //Comprueba si hay say
    nom_user = substring(s,12, strlen(s));
    if (strcmp(nom_user,"")==0) return 0;
    return controlError(nom_user, "a", 5);
  }
  else if(strcmp(substring(s, 0, 8), STRING_6) == 0) {
    nom_user = readTillChar(s,' ',' ');
    audio = readTillCharDouble(s,' ','\n', 2); //DOble es que lee desde el segundo espacio.
    return controlError(nom_user, audio, 6);
  }
  else if(strcmp(substring(s, 0, 4), STRING_7) == 0) {
    write(1, DISCONNECTING, strlen(DISCONNECTING));
    liberaMemoria(user, s);
    return 7;
  }
  return 0;
}

int INOUT_eligeOpcion(User * user) {
  int c = '\0';
  int i = 0;
  char * cadena = malloc (sizeof(char));
  while(c != '\n') {
    read(0, &c, sizeof(char));
    if(c != '\n') {
      cadena = realloc(cadena, sizeof(char) * (i + 2));
      cadena[i] = c;
    }
    i++;
  }
  cadena[i - 1] = '\0';
  for(i = 0; cadena[i]; i++){
      cadena[i] = toupper(cadena[i]);   //Pasamos a mayusculas.
  }

  return checkString(user, cadena);
}
