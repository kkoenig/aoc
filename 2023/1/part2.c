#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// --- Part Two ---
// Your calculation isn't quite right. It looks like some of the digits are
// actually spelled out with letters: one, two, three, four, five, six, seven,
// eight, and nine also count as valid "digits".
//
// Equipped with this new information, you now need to find the real first and
// last digit on each line. For example:
//
// two1nine
// eightwothree
// abcone2threexyz
// xtwone3four
// 4nineeightseven2
// zoneight234
// 7pqrstsixteen
//
// In this example, the calibration values are 29, 83, 13, 24, 42, 14, and 76.
// Adding these together produces 281.
//
// What is the sum of all of the calibration values?

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
    }
  }
  return false;
}

int main(void) {
  size_t calibration_total = 0;

  char *buffer = NULL;
  size_t buffer_size;
  ssize_t line_size;
  while (-1 != (line_size = getline(&buffer, &buffer_size, stdin))) {
    int first_digit;
    ssize_t i = 0;
    for (; i < line_size; ++i) {
      if (is_digit(buffer + i, &first_digit)) {
        break;
      }
    }

    int final_digit = first_digit;
    for (; i < line_size; ++i) {
      int digit;
      if (is_digit(buffer + i, &digit)) {
        final_digit = digit;
      }
    }
    calibration_total += first_digit * 10 + final_digit;
  }
  printf("%zu", calibration_total);
  free(buffer);
  return 0;
}
