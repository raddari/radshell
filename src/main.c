#include "rsio.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define LINE_LIMIT 256


static int fork_and_execute(const struct Command *cmd);
static int change_dir(const struct Command *cmd);
static void print_command(const struct Command *cmd);


int main(int argc, char **argv) {
  FILE *in;
  // TODO(raddari): Handle args properly
  if (argc == 2) {
    in = fopen(argv[1], "r");
    if (!in) {
      fprintf(stderr, "Invalid file: %s\n", argv[1]);
      return EXIT_FAILURE;
    }
  } else {
    in = stdin;
  }

  // FIXME(raddari): !feof(stdin) kinda borked
  while (!feof(in)) {
    char line[LINE_LIMIT];
    rs_read_line(line, sizeof line, in);
    struct Command cmd = rs_command_from_str(line);

#ifdef DEBUG
    print_command(&cmd);
#endif

    int result;
    if (strcmp("cd", cmd.argv[0]) == 0) {
      // TODO(raddari): implement cd
      result = change_dir(&cmd);
    } else {
      result = fork_and_execute(&cmd);
    }
    rs_command_free_args(&cmd);
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

static int change_dir(const struct Command *cmd) {
  if (cmd->argc > 1) {
    return chdir(cmd->argv[1]);
  } else {
    // TODO(raddari): Proper error code
    return 0;
  }
}

static void print_command(const struct Command *cmd) {
  printf("Command{argc=%d, argv={\n", cmd->argc);
  for (int i = 0; i < cmd->argc; i++) {
    printf("    \"%s\",\n", cmd->argv[i]);
  }
  printf("  }\n}\n");
}
