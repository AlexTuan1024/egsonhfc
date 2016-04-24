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

int main() {

  char *advices[] = {
      "You're haunting.\r\n",
      "You look like an avocado having sex with an older avocado.\r\n",
      "You look like testicles with teeth.\r\n"};

  /*
  ** open socket
  **/
  int listener_d = socket(PF_INET, SOCK_STREAM, 0);
  if (listener_d == -1) {
    error("Can't open socket");
  }
  int reuse = 1;
  if (setsockopt(listener_d, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse,
                 sizeof(int)) == -1)
    error("Can't set the reuse option on the socket");
  /*
  ** Bind port
  **/
  struct sockaddr_in name;
  name.sin_family = PF_INET;
  name.sin_port = (in_addr_t)htons(30000);
  name.sin_addr.s_addr = htonl(INADDR_ANY);
  int c = bind(listener_d, (struct sockaddr *)&name, sizeof(name));
  if (c == -1)
    error("Can't bind port");

  /*
  ** listen port
  **/
  if (listen(listener_d, 10) == -1)
    error("Can't listen port");
  puts("waiting for connection...");

  while (1) {
    /*
    ** accept connection
    **/
    struct sockaddr_storage client_addr;
    unsigned int address_size = sizeof(client_addr);
    int connect_d =
        accept(listener_d, (struct sockaddr *)&client_addr, &address_size);
    if (connect_d == -1)
      error("Can't open storage socket");

    char *msg = advices[random() % 3];
    if (send(connect_d, msg, strlen(msg), 0) == -1)
      error("Send message error");
    close(connect_d);
  }

  return 0;
}