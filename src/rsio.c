#include "rsio.h"

#include <stdlib.h>
#include <string.h>


char* rs_read_line(char *buffer, int count, FILE *stream) {
  fgets(buffer, count, stream);
  char *newline = strchr(buffer, '\n');
  if (newline) {
    *newline = '\0';
  }
  return buffer;
}

struct Command rs_command_from_str(const char *str) {
  char buffer[256];
  strncpy(buffer, str, sizeof buffer);

  // FIXME(raddari): what if the line is blank?
  struct Command cmd = { .argc = 1, .argv = NULL };
  char *token = strtok(buffer, " ");
  while ((token = strtok(NULL, " "))) {
    cmd.argc++;
  }
  // malloc 1 more arg for NULL terminator
  cmd.argv = malloc((cmd.argc + 1) * sizeof *cmd.argv);
  cmd.argv[cmd.argc] = NULL;

  token = buffer;
  for (int i = 0; i < cmd.argc; i++) {
    cmd.argv[i] = malloc(strlen(token) + 1);
    strcpy(cmd.argv[i], token);
    token += strlen(token) + 1; // Advance past NULL terminator
  }

  return cmd;
}

void rs_command_free_args(struct Command *command) {
  for (int i = 0; i < command->argc; i++) {
    free(command->argv[i]);
    command->argv[i] = NULL;
  }
  free(command->argv);
  command->argv = NULL;
}
