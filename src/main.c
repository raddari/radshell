#include "rsio.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define PROMPT "> "
#define LINE_LIMIT 256


static int fork_and_execute(const struct Command *cmd);
static void print_command(const struct Command *cmd);


int main() {
  // FIXME(raddari): !feof(stdin) kinda borked
  while (!feof(stdin)) {
    printf(PROMPT);

    char line[LINE_LIMIT];
    rs_read_line(line, sizeof line, stdin);
    struct Command cmd = rs_command_from_str(line);

#ifdef DEBUG
    print_command(&cmd);
#endif

    if (strcmp("cd", cmd.argv[0]) == 0) {
      /// TODO(raddari): implement cd
    } else {
      int result = fork_and_execute(&cmd);
    }
  }
  return EXIT_SUCCESS;
}

static int fork_and_execute(const struct Command *cmd) {
    int pid = fork();
    if (pid > 0) {
      int wstatus;
      waitpid(pid, &wstatus, 0);
      if (WIFEXITED(wstatus)) {
        return WEXITSTATUS(wstatus);
      } else {
        return -1;
      }
    } else if (pid == 0) {
      int result = execvp(cmd->argv[0], cmd->argv);
      perror("execvp"); // Not reached if exec succeeds
      return result;
    } else {
      perror("fork");
      return pid;
    }
}

static void print_command(const struct Command *cmd) {
  printf("Command{argc=%d, argv={\n", cmd->argc);
  for (int i = 0; i < cmd->argc; i++) {
    printf("    \"%s\",\n", cmd->argv[i]);
  }
  printf("  }\n}\n");
}
