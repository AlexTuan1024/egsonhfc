#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void diediedie(int);
int catch_signal(int, void (*)(int));
int main() {
  if (catch_signal(SIGINT, diediedie) == -1) {
    fprintf(stderr, "Can't map the handler\n");
    exit(2);
  }
  char name[30];
  printf("Enter your name:");
  fgets(name, sizeof(name) / sizeof(char), stdin);
  if (name[strlen(name) - 1] == '\n')
    name[strlen(name) - 1] = 0;
  printf("Hello %s\n", name);
  return 0;
}
void diediedie(int sig) {
  puts("Goodbye cruel world...\n");
  exit(1);
}
int catch_signal(int sig, void (*handler)(int)) {
  struct sigaction action;
  action.sa_handler = handler;
  sigemptyset(&action.sa_mask);
  action.sa_flags = 0;
  return sigaction(sig, &action, NULL);
}