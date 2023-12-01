#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool is_digit(const char *s, int *out_digit) {
  const char *digit_strings[9] = {
      "one", "two", "three", "four", "five", "six", "seven", "eight", "nine",
  };

  if (s[0] >= '1' && s[0] <= '9') {
    *out_digit = s[0] - '0';
    return true;
  }

  for (int i = 0; i < 9; ++i) {
    if (0 == strncmp(digit_strings[i], s, strlen(digit_strings[i]))) {
      *out_digit = i + 1;
      return true;
      break;
    }
  }
  return false;
}

int main(void) {
  char *buffer = NULL;
  size_t buffer_size;

  size_t total = 0;
  ssize_t line_size;
  while (-1 != (line_size = getline(&buffer, &buffer_size, stdin))) {
    int first_digit = -1;
    int last_digit = 0;
    for (ssize_t i = 0; i < line_size; ++i) {
      int digit;
      if (is_digit(buffer + i, &digit)) {
        if (first_digit == -1) {
          first_digit = digit;
        }
        last_digit = digit;
      }
    }
    total += first_digit * 10 + last_digit;
  }
  printf("%zu\n", total);
  free(buffer);
  return 0;
}
