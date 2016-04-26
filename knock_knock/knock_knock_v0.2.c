#include <arpa/inet.h>
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

/*
** common error function
**/
void error(char *msg) {
  fprintf(stderr, "%s:%s\n", msg, strerror(errno));
  exit(1);
}

/*
** open socket
**/
int open_socket_listener() {
  int s = socket(PF_INET, SOCK_STREAM, 0);
  if (s == -1)
    error("Can't open socket");
  return s;
}

/*
** bind port
**/
void bind_to_port(int socket, int port) {
  struct sockaddr_in name;
  name.sin_family = PF_INET;
  name.sin_port = (in_port_t)htons(port);
  name.sin_addr.s_addr = htonl(INADDR_ANY);
  int reuse = 1;
  if (setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse,
                 sizeof(int)) == -1)
    error("Can't set the reuse option on the socket");
  int c = bind(socket, (struct sockaddr *)&name, sizeof(name));
  if (c == -1)
    error("Can't bind to socket");
}

/*
** Send string to client
**/
int say(int socket, char *s) {
  int result = send(socket, s, strlen(s), 0);
  if (result == -1) {
    error("Error talking to the client");
  }
  return result;
}

/*
** reading from the client
**/
int read_in(int socket, char *buf, int len) {
  char *s = buf;
  int slen = len;
  int c = recv(socket, s, slen, 0);
  while (c > 0 && s[c - 1] != '\n') {
    // move s
    s += c;
    slen -= c;
    c = recv(socket, s, slen, 0);
  }

  if (c < 0)
    return c;
  if (c == 0)
    buf[0] = '\0';
  else
    s[c - 1] = '\0';
  return len - slen;
}

int catch_signal(int sig, void (*handler)(int)) {
  struct sigaction action;
  action.sa_handler = handler;
  sigemptyset(&action.sa_mask);
  action.sa_flags = 0;
  return sigaction(sig, &action, NULL);
}

int listener_d;
void handle_shutdown(int sig) {
  if (sig == SIGINT) {
    if (listener_d)
      close(listener_d);

    fprintf(stderr, "Bye\n");
    exit(0);
  }
}

int main() {
  char buf[255];
  if (catch_signal(SIGINT, handle_shutdown) == -1)
    error("Can't set the interrupt handler");
  listener_d = open_socket_listener();
  bind_to_port(listener_d, 30000);
  if (listen(listener_d, 10) == -1)
    error("Can't listen");
  puts("Waiting for connection...");

  while (1) {
    struct sockaddr_storage client_addr;
    unsigned int address_size = sizeof(client_addr);
    int connect_d =
        accept(listener_d, (struct sockaddr *)&client_addr, &address_size);
    if (connect_d == -1)
      error("Can't open storage socket");
    int pid = fork();
    if (pid == -1)
      error("Fork process error");
    if (!pid) {
      close(listener_d);
      if (say(connect_d, "Internet Knock-Knock Protocol Server\r\nVersion "
                         "1.0\r\nKnock!Knock!\r\n>") != 1) {
        read_in(connect_d, buf, sizeof(buf));
        if (strncasecmp("Who's there?", buf, 12))
          say(connect_d, "You should say 'Who's there?'!");
        else {
          if (say(connect_d, "Oscar\r\n>") != -1) {
            read_in(connect_d, buf, sizeof(buf));
            if (strncasecmp("Oscar who?", buf, 10))
              say(connect_d, "You should say 'Oscar who?'!");
            else
              say(connect_d, "Oscar silly question,you get a silly answer\r\n");
          }
        }
      }
      close(connect_d);
      exit(0);
    }
    close(connect_d);
  }
  return 0;
}