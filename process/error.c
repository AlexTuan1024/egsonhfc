#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main() {
  if (execlp("kryptonite", "kryptonite", NULL) == -1) {
    fprintf(stderr, "Can not run ipconfig:%s\n", strerror(errno));
    perror("It truely is");
    return 1;
  }
  return 0;
}