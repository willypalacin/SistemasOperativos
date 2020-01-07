#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

void dondeEstaElError(char * pa1, char * pa2) {
  int fd;
  int fd2;
  int i = 0;
  char c;
  char c2;

  fd = open(pa1, O_RDONLY);
  fd2 = open(pa2, O_RDONLY);


  for (i = 0; i < 120000; i++){
    read(fd, &c, 1);
    read(fd2, &c2, 1);
    if(c != c2) {
      printf("Difiere el caracter %d, c1: %c c2: %c\n", i, c, c2);
    }
  }

}

int main(int argc, char **argv) {
  dondeEstaElError("./Audios1/a.mp3", "./Audios2/a.mp3");
  /* code */
  return 0;
}
