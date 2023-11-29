#include <stdio.h>

// --- Part Two ---
// Now, given the same instructions, find the position of the first character
// that causes him to enter the basement (floor -1). The first character in the
// instructions has position 1, the second character has position 2, and so on.
//
// For example:
//
// - ) causes him to enter the basement at character position 1.
// - ()()) causes him to enter the basement at character position 5.
//
// What is the position of the character that causes Santa to first enter the
// basement?
int main(void) {
  int floor = 0;
  for (int i = 0, ch; (ch = getchar()) != EOF; ++i) {
    floor -= (ch - '(') * 2 - 1;
    if (floor < 0) {
      printf("%d\n", i + 1);
      break;
    }
  }
  return 0;
}
