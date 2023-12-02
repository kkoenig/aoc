#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// --- Part Two ---
// The engineer finds the missing part and installs it in the engine! As the
// engine springs to life, you jump in the closest gondola, finally ready to
// ascend to the water source.
//
// You don't seem to be going very fast, though. Maybe something is still wrong?
// Fortunately, the gondola has a phone labeled "help", so you pick it up and
// the engineer answers.
//
// Before you can explain the situation, she suggests that you look out the
// window. There stands the engineer, holding a phone in one hand and waving
// with the other. You're going so slowly that you haven't even left the
// station. You exit the gondola.
//
// The missing part wasn't the only issue - one of the gears in the engine is
// wrong. A gear is any * symbol that is adjacent to exactly two part numbers.
// Its gear ratio is the result of multiplying those two numbers together.
//
// This time, you need to find the gear ratio of every gear and add them all up
// so that the engineer can figure out which gear needs to be replaced.
//
// Consider the same engine schematic again:
//
// 467..114..
// ...*......
// ..35..633.
// ......#...
// 617*......
// .....+.58. ..592.....
// ......755.
// ...$.*....
// .664.598..
// In this schematic, there are two gears. The first is in the top left; it has
// part numbers 467 and 35, so its gear ratio is 16345. The second gear is in
// the lower right; its gear ratio is 451490. (The * adjacent to 617 is not a
// gear because it is only adjacent to one part number.) Adding up all of the
// gear ratios produces 467835.
//
// What is the sum of all of the gear ratios in your engine schematic?
int main(void) {
  char *buffer = NULL;
  size_t buffer_size;
  ssize_t line_size;

  // Loop through input data to get row count and column width
  int rows = 0;
  int cols = 0;
  while (-1 != (line_size = getline(&buffer, &buffer_size, stdin))) {
    cols = line_size - 1;
    ++rows;
  };
  rewind(stdin);

  typedef struct {
    // original schematic value
    char v;
    // ..522..123.. -> 001110022200
    // nth number index (0 for numbers)
    int number_index;
  } Cell;
  Cell *table = calloc(1, sizeof(Cell) * rows * cols);

#define MAX_SCHEMATIC_NUMBERS 10000
  // List of all numbers that appear in the schematic
  int schematic_numbers[MAX_SCHEMATIC_NUMBERS] = {0};
  int total_numbers = 1;
  for (int r = 0; -1 != getline(&buffer, &buffer_size, stdin); ++r) {
    for (int c = 0; c < cols;) {
      if (isdigit(buffer[c])) {
        int number = 0;
        while (c < cols && isdigit(buffer[c])) {
          number = 10 * number + buffer[c] - '0';
          table[r * cols + c] = (Cell){
              .v = buffer[c],
              .number_index = total_numbers,
          };
          ++c;
        }
        schematic_numbers[total_numbers++] = number;
      } else {
        table[r * cols + c] = (Cell){.v = buffer[c]};
        ++c;
      }
    }
  }

  int parts_seen[MAX_SCHEMATIC_NUMBERS] = {0};
  ssize_t total_gear_ratios = 0;
  for (int r = 0; r < rows; ++r) {
    for (int c = 0; c < cols; ++c) {
      int ch = table[r * cols + c].v;
      if (ch == '.' || isdigit(ch)) {
        continue;
      }

      int count = 0;
      int product = 1;
      for (int y = r - 1; y <= r + 1; y += 1) {
        for (int x = c - 1; x <= c + 1; x += 1) {
          if (y < 0 || y >= rows || x < 0 || x >= cols) {
            continue;
          }
          parts_seen[table[y * cols + x].number_index] = 0;
        }
      }
      // Check all adjacent cells
      // Don't need exclude (x, y) since parts_seen starts at 1 so we will
      // just end up adding zero below
      for (int y = r - 1; y <= r + 1; y += 1) {
        for (int x = c - 1; x <= c + 1; x += 1) {
          if (y < 0 || y >= rows || x < 0 || x >= cols) {
            continue;
          }
          Cell *cell = &table[y * cols + x];

          if (cell->number_index && !parts_seen[cell->number_index]) {
            ++count;
            product *= schematic_numbers[cell->number_index];
            parts_seen[cell->number_index] = 1;
          }
        }
      }
      if (count == 2) {
        total_gear_ratios += product;
      }
    }
  }

  printf("%zd", total_gear_ratios);
  free(table);
  free(buffer);
  return 0;
}
