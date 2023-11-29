#include <stdio.h>

int main() {
  int floor = 0;
  for (int ch; (ch = getchar()) != EOF;) {
    floor -= (ch - '(') * 2 - 1;
  }

  printf("%d\n", floor);
  return 0;
}
