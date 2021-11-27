#include "rsio.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define PROMPT "> "
#define LINE_LIMIT 256


static void print_command(const struct Command *cmd);


int main() {
  printf(PROMPT);

  char line[LINE_LIMIT];
  rs_read_line(line, sizeof line, stdin);
  struct Command cmd = rs_command_from_str(line);
  print_command(&cmd);

  int pid = fork();
  if (pid > 0) {
    int status;
    waitpid(pid, &status, 0);
  } else if (pid == 0) {
    execvp(cmd.argv[0], cmd.argv);
    perror("execvp"); // Not reached if exec succeeds
  } else {
    perror("fork");
  }
  return EXIT_SUCCESS;
}

static void print_command(const struct Command *cmd) {
  printf("Command{argc=%d, argv={\n", cmd->argc);
  for (int i = 0; i < cmd->argc; i++) {
    printf("    \"%s\",\n", cmd->argv[i]);
  }
  printf("  }\n}\n");
}
