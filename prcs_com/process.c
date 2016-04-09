#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

void error(char *info);
int main(int argc, char *argv[]) {
  printf("begin\n");
  time_t rawtime;
  struct tm *timeinfo;
  
  int pid_status;
  char *phrase = argv[1];
  char *vars[] = {"RSS_FEED=fake", NULL};
  FILE *f = fopen("stories.txt", "w");
  if (!f) {
    error("Can't open stories.txt");
  }
  pid_t pid = fork();
  if (pid == -1) {
    error("Can't fork process");
  }
  if (pid == 0) {
    if (dup2(fileno(f), 1) == -1) {
      error("Can't redirect Standard Output");
    }
    if (execle("/usr/bin/python", "/usr/bin/python", "./fake_rss.py", phrase,
               NULL, vars) == -1) {
      error("Can't run script");
    }
  }
  
  if(waitpid(pid,&pid_status,0) == -1){
    error("Wait child process error");
  }else
    printf("Daddy got ya!\n");
    fclose(f);
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    printf("The current data/time is:%s\n",asctime(timeinfo));
    if(!WEXITSTATUS(pid_status)){
      printf("Ends well\n");
    }
  return 0;
}

void error(char *info) {
  fprintf(stderr, "%s %s\n", info, strerror(errno));
  exit(1);
}