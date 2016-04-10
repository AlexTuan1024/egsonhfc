#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

void error(char *);
void open_url(char *);
int main(int argc, char *argv[]) {
  int pid_status, pid2_status;
  int fd[2];
  if (pipe(fd) == -1) {
    error("Can't create pipe");
  }
  pid_t pid = fork();
  if (pid == -1) {
    error("Can't fork process");
  }
  if (pid == 0) {
    
    dup2(fd[1], 1);
    close(fd[0]);
    execlp("python", "python", "./fake_rss.py", NULL);
  }

  // Parent proces

  dup2(fd[0], 0);
  close(fd[1]);
  char line[255];

  while (fgets(line, sizeof(line) / sizeof(char), stdin)) {
    if (line[strlen(line) - 1] == '\n')
      line[strlen(line) - 1] = 0;
    printf("Got url :%s\n", line);

    pid_t pid2 = fork();
    if (pid2 == 0) {
      open_url(line);
    }
    if (waitpid(pid2, &pid2_status, 0) == -1) {
      error("wait pid2 error");
    } else {
      printf("open url succeeded\n");
    }
  }

  if (waitpid(pid, &pid_status, 0) == -1) {
    error("wait pid error");
  } else {
    printf("Script have executed successfully\n");
  }
  return 0;
}

void error(char *message) {
  fprintf(stderr, "%s: %s\n", message, strerror(errno));
}
void open_url(char *url) {
  if (execlp("open", "open", url, NULL) == -1) {
    error("Can't open url");
  }
}