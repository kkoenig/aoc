#include <stdio.h>
// --- Day 3: Perfectly Spherical Houses in a Vacuum ---
// Santa is delivering presents to an infinite two-dimensional grid of houses.
//
// He begins by delivering a present to the house at his starting location, and
// then an elf at the North Pole calls him via radio and tells him where to move
// next. Moves are always exactly one house to the north (^), south (v), east
// (>), or west (<). After each move, he delivers another present to the house
// at his new location.
//
// However, the elf back at the north pole has had a little too much eggnog, and
// so his directions are a little off, and Santa ends up visiting some houses
// more than once. How many houses receive at least one present?
//
// For example:
//
// - > delivers presents to 2 houses: one at the starting location, and one to
// the east.
// - ^>v< delivers presents to 4 houses in a square, including twice to the
// house at his starting/ending location. ^v^v^v^v^v delivers a bunch of
// presents to some very lucky children at only 2 houses.

int main(void) {
#define MAX_COORD 1000
  int x = MAX_COORD, y = MAX_COORD;
  char seen[4 * MAX_COORD * MAX_COORD] = {0};
  seen[(x * 2 * MAX_COORD) + y] = 1;
  char ch;
  while ((ch = getchar()) != EOF) {
    switch (ch) {
    case '^':
      --y;
      break;
    case 'v':
      ++y;
      break;
    case '>':
      ++x;
      break;
    case '<':
      --x;
      break;
    }

    seen[y * 2 * MAX_COORD + x] = 1;
  }

  int total_seen = 0;
  for (size_t i = 0; i < sizeof(seen) / sizeof(char); ++i) {
    total_seen += seen[i];
  }
  printf("%d", total_seen);
  return 0;
}
