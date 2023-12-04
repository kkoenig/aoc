#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main(void) {
  int total = 0;
  char *buffer = NULL;
  size_t buffer_size;
  ssize_t line_size;

  // int i = 1;
  while (-1 != (line_size = getline(&buffer, &buffer_size, stdin))) {
    int winners[1000] = {0};

    int j = 0;
    while (buffer[j] != ':') {
      ++j;
    }
    ++j;
    while (' ' == buffer[j]) {
      ++j;
    }

    while (buffer[j] != '|') {
      int n = 0;
      while (isdigit(buffer[j])) {
        n = n * 10 + buffer[j] - '0';
        ++j;
      }
      winners[n] = 1;

      while (' ' == buffer[j]) {
        ++j;
      }
    }

    ++j;
    while (' ' == buffer[j]) {
      ++j;
    }

    int winning = 0;
    while (j < line_size - 1) {
      int n = 0;
      while (isdigit(buffer[j])) {
        n = n * 10 + buffer[j] - '0';
        ++j;
      }
      if (winners[n]) {
        ++winning;
      }

      while (' ' == buffer[j]) {
        ++j;
      }
    }
    if (winning) {
      total += 1 << (winning - 1);
    }
  }
  free(buffer);
  printf("%u", total);
  return 0;
}
