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
    (*user).q = i+1;


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
  for (i = 0; i < (*user).q; i++) {
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
  if (strcmp(s, STRING_1) == 0) return 1;
  else if (strcmp(substring(s, 0, 7), STRING_2_1) == 0) {      //Comprueba si hay escrita la palabra CONNECT.
    opcion = checkStringCase2(user, s);
    printf("LA OPCION ES: %d \n", opcion);
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
