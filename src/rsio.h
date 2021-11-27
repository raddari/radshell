#pragma once

#include <stdio.h>


struct Command {
  int argc;
  char **argv;
};

char* rs_read_line(char *buffer, int count, FILE *stream);
struct Command rs_command_from_str(const char *str);
void rs_command_free_args(struct Command *command);
