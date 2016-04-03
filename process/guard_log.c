#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

char *now() {
  time_t t;
  time(&t);
  return asctime(localtime(&t));
}

/*main function:get the record,then create a piece of command for system call*/
int main() {
  char comments[80];
  char cmd[120];
  fgets(comments, 80, stdin);
  if (comments[strlen(comments) - 1] == '\n')
    comments[strlen(comments) - 1] = 0;
  sprintf(cmd, "echo %s %s >> reports.log", comments, now());
  system(cmd);
  return 0;
}