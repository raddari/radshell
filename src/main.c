#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

#define PROMPT "> "
#define LINE_LIMIT 256


int main() {
  printf(PROMPT);

  char line[LINE_LIMIT];
  fgets(line, sizeof line, stdin);

  printf("User typed: %s\n", line);

  int pid = fork();
  if (pid > 0) {
    // Parent
    int status;
    waitpid(pid, &status, 0);
  } else if (pid == 0) {
    // Child
    char *args[] = {
      line,
      NULL,
    };
    execvp(line, args);
    perror("execvp"); // Not reached if exec succeeds
  } else {
    // Error
    perror("fork");
  }
  return 0;
}
