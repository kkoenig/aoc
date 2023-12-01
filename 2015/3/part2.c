#include <stdio.h>
// --- Part Two ---
// The next year, to speed up the process, Santa creates a robot version of
// himself, Robo-Santa, to deliver presents with him.
//
// Santa and Robo-Santa start at the same location (delivering two presents to
// the same starting house), then take turns moving based on instructions from
// the elf, who is eggnoggedly reading from the same script as the previous
// year.
//
// This year, how many houses receive at least one present?
//
// For example:
//
// - ^v delivers presents to 3 houses, because Santa goes north, and then
// Robo-Santa goes south.
// - ^>v< now delivers presents to 3 houses, and Santa and Robo-Santa end up
// back where they started.
// -  ^v^v^v^v^v now delivers presents to 11 houses, with Santa
// going one direction and Robo-Santa going the other.
int main(void) {
#define MAX_COORD 200
  int rx = MAX_COORD, ry = MAX_COORD;
  int sx = rx, sy = ry;
  int index = 0;

  char seen[4 * MAX_COORD * MAX_COORD] = {0};
  seen[(sy * 2 * MAX_COORD) + sx] = 1;

  char ch;
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
