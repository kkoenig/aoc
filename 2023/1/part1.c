#include <stdio.h>
#include <stdlib.h>
// --- Day 1: Trebuchet?! ---
// Something is wrong with global snow production, and you've been selected to
// take a look. The Elves have even given you a map; on it, they've used stars
// to mark the top fifty locations that are likely to be having problems.
//
// You've been doing this long enough to know that to restore snow operations,
// you need to check all fifty stars by December 25th.
//
// Collect stars by solving puzzles. Two puzzles will be made available on each
// day in the Advent calendar; the second puzzle is unlocked when you complete
// the first. Each puzzle grants one star. Good luck!
//
// You try to ask why they can't just use a weather machine ("not powerful
// enough") and where they're even sending you ("the sky") and why your map
// looks mostly blank ("you sure ask a lot of questions") and hang on did you
// just say the sky ("of course, where do you think snow comes from") when you
// realize that the Elves are already loading you into a trebuchet ("please hold
// still, we need to strap you in").
//
// As they're making the final adjustments, they discover that their calibration
// document (your puzzle input) has been amended by a very young Elf who was
// apparently just excited to show off her art skills. Consequently, the Elves
// are having trouble reading the values on the document.
//
// The newly-improved calibration document consists of lines of text; each line
// originally contained a specific calibration value that the Elves now need to
// recover. On each line, the calibration value can be found by combining the
// first digit and the last digit (in that order) to form a single two-digit
// number.
//
// For example:
//
// 1abc2
// pqr3stu8vwx
// a1b2c3d4e5f
// treb7uchet

// In this example, the calibration values of these four lines are 12, 38, 15,
// and 77. Adding these together produces 142.
//
// Consider your entire calibration document. What is the sum of all of the
// calibration values?
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
