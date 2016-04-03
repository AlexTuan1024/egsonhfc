#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main() {
  char ext[] = "diner_info";
  char *myenv[] = {"JUICE=peach and apple","SUPERMAN=Clark Kent", NULL}; // it also ends up with NULL
  execle(ext, ext, "4", NULL, myenv);
  printf("run diner_info wrong!\n");
  return 0;
}