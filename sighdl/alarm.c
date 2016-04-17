#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

int score = 0;
void end_game(int sig) {
  printf("\nFinal Score:%i\n", score);
  exit(0);
}
int catch_signal(int sig, void (*handler)(int)) {
  struct sigaction action;
  action.sa_handler = handler;
  sigemptyset(&action.sa_mask);
  action.sa_flags = 0;
  return sigaction(sig, &action, NULL);
}
void times_up(int sig) {
  puts("\nTIME'S UP!");
  raise(SIGINT);
}
void error(char *msg) {
  fprintf(stderr, "%s %s\n", msg, strerror(errno));
  exit(1);
}
int main() {
  if (catch_signal(SIGALRM, times_up) == -1) {
    error("alarm handler error");
  }
  if (catch_signal(SIGINT, end_game) == -1) {
    error("interrupt handler error");
  }
  srandom(time(0));
  while (1) {
    int a = random() % 11;
    int b = random() % 11;
    char txt[4];
    // set an alarm for 5 seconds
    alarm(5);
    printf("\nWhat is %i times %i?", a, b);
    fgets(txt, 4, stdin);
    if (txt[strlen(txt) - 1] == '\n')
      txt[strlen(txt) - 1] = 0;
    int answer = atoi(txt);
    if (answer == a * b) {
      score++;
    } else {
      printf("\nWrong!Score:%i\n", score);
    }
  }
  return 0;
}