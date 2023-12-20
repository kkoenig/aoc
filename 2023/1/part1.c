#include <stdio.h>
#include <stdlib.h>
int main(void) {
  size_t calibration_total = 0;

  char *buffer = NULL;
  size_t buffer_size;
  ssize_t line_size;
  while (-1 != (line_size = getline(&buffer, &buffer_size, stdin))) {
    // Assuming valid input we look for the first digit
    ssize_t i = 0;
    for (; i < line_size; ++i) {
      if (buffer[i] >= '1' && buffer[i] <= '9') {
        break;
      }
    }

    // Then update final digit as we go through the rest of the line
    int first_digit = buffer[i] - '0';
    int final_digit = first_digit;

    for (; i < line_size; ++i) {
      char c = buffer[i];
      if (c >= '1' && c <= '9') {
        final_digit = c - '0';
      }
    }
    calibration_total += first_digit * 10 + final_digit;
  }
  printf("%zu", calibration_total);
  free(buffer);
  return 0;
}
