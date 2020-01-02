#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

#define WRITE_END 1
#define READ_END 0

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
      write(1, buffer, strlen(buffer));
      close(fd[READ_END]);
  }
  return buffer;
}

int readFile(char * path){
  int fd,retval;
  char * buffer = malloc(sizeof(char));
  char * buffer2 = malloc(sizeof(char));

  /*opening the file in read-only mode*/
  if ((fd = open(path, O_RDONLY)) < 0) {
      perror("Problem in opening the file");
      exit(1);
  }

  /*reading bytes from the fd and writing it to the buffer*/
  int i = 0;
  while ((retval = read(fd, buffer, 1)) > 0){
      buffer2[i] = *buffer;
      buffer2= realloc(buffer2, i+2);
      i++;
  }
  if (retval < 0) {
      perror("\nRead failure");
      exit(1);
  }

  close(fd);
  char newPath[128];
  sprintf(newPath, "./prueba/%s", path);
  int fd2 = open(newPath, O_RDWR);
  int w = 0;
  for(w = 0; w < i; w++ ){
    write(fd2, &buffer2[w], 1);
  }
  close(fd2);
  return 0;
}

int main(int argc, char **argv) {
 char newPath[128];
 sprintf(newPath, "./prueba/%s", argv[1]);
 readFile(argv[1]);
 printf("EL MD5 del archivo es: %s\n", ejecutaMD5(argv[1]));
 printf("EL MD5 del buffer es %s\n", ejecutaMD5(newPath));
}
