#include "rsio.h"

#include <string.h>


char* rs_read_line(char *buffer, int count, FILE *stream) {
  fgets(buffer, count, stream);
  char *newline = strchr(buffer, '\n');
  if (newline) {
    *newline = '\0';
  }
  return buffer;
}
