#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define PROMPT "> "
#define LINE_LIMIT 256


int main() {
  printf(PROMPT);

  char line[LINE_LIMIT];
  fgets(line, sizeof line, stdin);
  char *newline = strchr(line, '\n');
  if (newline) {
    *newline = '\0';
  }

  printf("User typed: %s\n", line);

  int pid = fork();
  if (pid > 0) {
    int status;
    waitpid(pid, &status, 0);
  } else if (pid == 0) {
    char *args[] = {
      line,
      NULL,
    };
    execvp(line, args);
    perror("execvp"); // Not reached if exec succeeds
  } else {
    perror("fork");
  }
  return EXIT_SUCCESS;
}
