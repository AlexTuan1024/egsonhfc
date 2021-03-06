#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  char *feeds[] = {"http://www.cnn.com/rss/celebs.xml",
                   "http://www.rollingstone.com/rock.xml",
                   "http://eonline.com/gossip.xml"};
  int times = 3;
  int i;
  char *phrase = argv[1];
  for (i = 0; i < times; i++) {
    char var[255];
    sprintf(var, "RSS_FEED=%s", feeds[i]);
    char *vars[] = {var, NULL};
    pid_t pid;
    if ((pid = fork()) < 0) {
      fprintf(stderr, "Can't fork process: %s\n", strerror(errno));
      return 1;
    } else if (pid == 0) {
      if (execle("/usr/bin/python", "/usr/bin/python", "./fake_rss.py", phrase,
                 NULL, vars) == -1) {
        fprintf(stderr, "Can't run script: %s\n", strerror(errno));
        return 1;
      }
    }
  }
  return 0;
}