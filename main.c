#include <asm-generic/ioctls.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>

int main(int argc, char *argv[])
{

  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  printf("%u, %u\n", w.ws_row, w.ws_col);
  return EXIT_SUCCESS;
}
