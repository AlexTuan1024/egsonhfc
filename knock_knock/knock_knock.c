#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

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
  name.sin_port = (in_addr_t)htons(port);
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