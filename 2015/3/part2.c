#include <stdio.h>
int main(void) {
#define MAX_COORD 200
  int rx = MAX_COORD, ry = MAX_COORD;
  int sx = rx, sy = ry;
  int index = 0;

  char seen[4 * MAX_COORD * MAX_COORD] = {0};
  seen[(sy * 2 * MAX_COORD) + sx] = 1;

  int ch;
  while ((ch = getchar()) != EOF) {
    int *x, *y;
    if (index & 1) {
      x = &rx;
      y = &ry;
    } else {
      x = &sx;
      y = &sy;
    }
    switch (ch) {
    case '^':
      --*y;
      break;
    case 'v':
      ++*y;
      break;
    case '>':
      ++*x;
      break;
    case '<':
      --*x;
      break;
    }
    seen[*y * 2 * MAX_COORD + *x] = 1;
    ++index;
  }

  int total_seen = 0;
  for (size_t i = 0; i < sizeof(seen) / sizeof(char); ++i) {
    total_seen += seen[i];
  }
  printf("%d", total_seen);
  return 0;
}
