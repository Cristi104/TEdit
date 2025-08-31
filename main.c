#include <asm-generic/ioctls.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <termios.h>

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
  raw.c_cflag |= CS8;
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

int main(int argc, char *argv[])
{
  enable_raw_mode();

  // struct winsize w;
  // ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  // printf("%u, %u\n", w.ws_row, w.ws_col);
  char input;
  while (read(STDIN_FILENO, &input, 1) == 1 && input != 'q') {
    printf("Read: %u\r\n", input);
  }

  return EXIT_SUCCESS;
}
