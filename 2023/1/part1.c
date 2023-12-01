#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

int main(void) {
  char *buffer = NULL;
  size_t buffer_size;

  ssize_t line_size;

  size_t total = 0;
  while (-1 != (line_size = getline(&buffer, &buffer_size, stdin))) {
    int first_digit = -1;
    int last_digit = 0;
    for (ssize_t i = 0; i < line_size; ++i) {
      if (isdigit(buffer[i])) {
        if (first_digit == -1) {
          first_digit = buffer[i] - '0';
        }
        last_digit = buffer[i] - '0';
      }
    }
    total += first_digit * 10 + last_digit;
  }
  printf("%zu\n", total);
  free(buffer);
  return 0;
}
