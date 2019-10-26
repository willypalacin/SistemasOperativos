#include "inout.h"

/*
This function reads until arrives to the end character.
The function returns the number of bytes readed.
*/
ssize_t readUntil(int fd, char end, char * string) {
  ssize_t n;
  int i = 0;
  char c = '\0';
  while (c != end) {
    n = read(fd, &c, sizeof(char));
    if(n == 0) {
      return 0;
    }
    printf("%c", c);
    if (c != end) {
      string = (char*)realloc(string, sizeof(char) * (i + 2));
      string[i] = c;
    }
    i++;
  }

  string[i - 1] = '\0';
  return n;
}

/*
Version inspirada en la funcion readUntil
pero cuando no haya file descriptor
Se da por hecho que el caracter siempre va a existir
*/
char * readTillChar(char* s, char init ,char end){
  printf("INIT es %c, end es %c\n", init, end );
  char * aux = malloc(sizeof(char)*200);
  aux[0]=' ';
  unsigned int i = 0;
  for(i = 0; i<strlen(s); i++) {
    if(s[i] == init) break;
  }
  i++;
  int j = 0;
  while (s[i] != end) {
    printf("AUX es %s\n", aux);
    aux[j] = s[i];
    i++; j++;
    if(i>=strlen(s)) break;
  }
  if(i>=strlen(s)) {
    aux[0]=' ';
    aux[1]='\0';
  }

  aux[i - 1] = '\0';
  printf("AUX es %s\n", aux);
  return aux;


}

void INOUT_readFile(char * nombre, User * user) {
  ssize_t n;
  int i = 0;
  int j;   //Number of conex
  int fd = open(nombre, O_RDONLY);
  if (fd < 0) {
    write(1, FILE_ERROR, strlen(FILE_ERROR));
  }
  else {
    n = readUntil(fd, END_CHAR, (*user).username);
    n = readUntil(fd, END_CHAR, (*user).audios);
    n = readUntil(fd, END_CHAR, (*user).ip);
    n = readUntil(fd, END_CHAR, (*user).port);

    while (n != 0) {
      n = readUntil(fd, END_CHAR, (*user).conex[i]);
      if (n == 0) break;
      (*user).conex = realloc ((*user).conex, sizeof(char *) * (i+2));
      (*user).conex[i+1] = malloc(sizeof(char));
      i++;
    }
    (*user).q_conex = i+1;


    printf("EL USERNAME ES %s\n", (*user).username);
    printf("CARPETA DE AUDIOS ES %s\n", (*user).audios);
    printf("LA IP ES %s\n", (*user).ip);
    printf("EL PUERTO ES %s\n", (*user).port);
    printf("LAS CONEX SON: %d\n", i );
    for (j = 0; j < i; j ++) {
      printf("Conex: %s\n", (*user).conex[j]);
    }
  }

}

/*
Comprueba todas las posibles puertos disponibles y
si coincide con CONNECT <puertoExistente> sera correcto
*/
int checkStringCase2(User * user, char * s) {
  char aux[100];
  int  i;
  for (i = 0; i < (*user).q_conex; i++) {
   sprintf(aux, "CONNECT %s", (*user).conex[i]);
   if (strcmp(aux, s) == 0) {
     return 2;
   }
 }
  return 0;
}

char * substring(char * s, unsigned int init , unsigned int end) {
  unsigned int i;
  char * aux = malloc(sizeof(char)*50);
  if (strlen(s)>end) {
    for (i = init; i < end; i++) {
      aux[i] = s[i];
    }
    return aux;
  }
  else {
    return " ";
  }
}

/*
Funcion que hace de menu. Comprueba las diferentes opciones.
*/
int checkString(User * user, char * s) {
  int opcion;
  char * nom_user;
  char * texto;
  if (strcmp(s, STRING_1) == 0) return 1;
  else if (strcmp(substring(s, 0, 7), STRING_2_1) == 0) {      //Comprueba si hay escrita la palabra CONNECT.
    opcion = checkStringCase2(user, s);
    return opcion;
  }
  else if(strcmp(substring(s, 0, 3), STRING_3) == 0) { //Comprueba si hay say
     nom_user = readTillChar(s,' ',' ');
     texto = readTillChar(s,'\"','\"');
     printf("EL USER es %s.\n", nom_user);
     printf("EL texto es %s.\n", texto);


  }
  return 1;

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

  return checkString(user, cadena);
}
