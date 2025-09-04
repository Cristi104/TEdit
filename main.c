#include <asm-generic/ioctls.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <termios.h>
#include <dirent.h>


struct termios original_attributes;

void disable_raw_mode(){
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &original_attributes);
}

void enable_raw_mode(){
  struct termios raw;
  atexit(disable_raw_mode);
  tcgetattr(STDIN_FILENO, &original_attributes);
  tcgetattr(STDIN_FILENO, &raw);
  raw.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);
  raw.c_oflag &= ~(OPOST);
  raw.c_lflag &= ~(ECHO | ICANON | ECHONL | ISIG | IEXTEN);
  raw.c_cflag &= ~(CSIZE | PARENB);
  raw.c_cflag |= CS8;
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

struct dirent* select_file(){
  write(STDOUT_FILENO, "\033[2J\033[3J\033[H", 12);
  struct dirent** filelist;
  struct dirent* file;
  int n;
  n = scandir(".", &filelist, NULL, alphasort);
  for(int i = 0; i < n; i++){
    file = filelist[i];
    write(STDOUT_FILENO, file->d_name, strlen(file->d_name));
    write(STDOUT_FILENO, "\r\n", 2);
  }
  write(STDOUT_FILENO, "\033[H", 4);

  char input[16];
  memset(input, 0, 16);
  while (read(STDIN_FILENO, &input, 15) >= 1 && strcmp(input, "q") != 0) {
    if(strcmp(input, "\033[A") == 0){
      write(STDOUT_FILENO, "\033[1A", 5);
      memset(input, 0, 16);
      continue;
    }

    if(strcmp(input, "\033[B") == 0){
      write(STDOUT_FILENO, "\033[1B", 5);
      memset(input, 0, 16);
      continue;
    }

    if(strcmp(input, "\r") == 0){
      write(STDOUT_FILENO, "\033[6n", 5);
      int line, collumn;
      scanf("\033[%u;%uR",&line, &collumn);
      if(line - 1 < n){
        file = malloc(sizeof(struct dirent)); 
        *file = *filelist[line - 1];
        free(filelist);
        write(STDOUT_FILENO, "\033[2J\033[3J\033[H", 12);
        return file;
      }
      memset(input, 0, 16);
      continue;
    }
  }
  write(STDOUT_FILENO, "\033[2J\033[3J\033[H", 12);
  free(filelist);
  return NULL;
}

int main(int argc, char *argv[])
{
  enable_raw_mode();
  struct dirent* file = select_file();
  write(STDOUT_FILENO, file->d_name, strlen(file->d_name));
  write(STDOUT_FILENO, "\r\n", 2);

  char input[16];
  memset(input, 0, 16);
  while (read(STDIN_FILENO, &input, 15) >= 1 && strcmp(input, "q") != 0) {
    if(strcmp(input, "a") == 0){
      write(STDOUT_FILENO, "\033[2J\033[3J\033[H", 12);
      memset(input, 0, 16);
      continue;
    }
    write(STDOUT_FILENO, input, 15);
    memset(input, 0, 16);
  }
  return EXIT_SUCCESS;
}
