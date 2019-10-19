#include "ficheros.h"

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

void FICHEROS_readFile(char * nombre, User * user) {
  ssize_t n;
  int i = 0;
  int j;   //Number of conex
  int fd = open(nombre, O_RDONLY);
  if (fd < 0) {
    write(1, FILE_ERROR, strlen(FILE_ERROR));
  }
  else {
    n =readUntil(fd, END_CHAR, (*user).username);
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
