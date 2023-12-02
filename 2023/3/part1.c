#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// --- Day 3: Gear Ratios ---
// You and the Elf eventually reach a gondola lift station; he says the gondola
// lift will take you up to the water source, but this is as far as he can bring
// you. You go inside.
//
// It doesn't take long to find the gondolas, but there seems to be a problem:
// they're not moving.
//
// "Aaah!"
//
// You turn around to see a slightly-greasy Elf with a wrench and a look of
// surprise. "Sorry, I wasn't expecting anyone! The gondola lift isn't working
// right now; it'll still be a while before I can fix it." You offer to help.
//
// The engineer explains that an engine part seems to be missing from the
// engine, but nobody can figure out which one. If you can add up all the part
// numbers in the engine schematic, it should be easy to work out which part is
// missing.
//
// The engine schematic (your puzzle input) consists of a visual representation
// of the engine. There are lots of numbers and symbols you don't really
// understand, but apparently any number adjacent to a symbol, even diagonally,
// is a "part number" and should be included in your sum. (Periods (.) do not
// count as a symbol.)
//
// Here is an example engine schematic:
//
// 467..114..
// ...*......
// ..35..633.
// ......#...
// 617*......
// .....+.58.
// ..592.....
// ......755.
// ...$.*....
// .664.598..
// In this schematic, two numbers are not part numbers because they are not
// adjacent to a symbol: 114 (top right) and 58 (middle right). Every other
// number is adjacent to a symbol and so is a part number; their sum is 4361.
//
// Of course, the actual engine schematic is much larger. What is the sum of all
// of the part numbers in the engine schematic?
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
  for (int r = 0; r < rows; ++r) {
    for (int c = 0; c < cols; ++c) {
      int ch = table[r * cols + c].v;
      if (ch == '.' || isdigit(ch)) {
        continue;
      }
      // Check all adjacent cells
      // Don't need exclude (x, y) since parts_seen starts at 1 so we will
      // just end up adding zero below
      for (int y = r - 1; y <= r + 1; y += 1) {
        for (int x = c - 1; x <= c + 1; x += 1) {
          if (y < 0 || y >= rows || x < 0 || x >= cols) {
            continue;
          }
          parts_seen[table[y * cols + x].number_index] = 1;
        }
      }
    }
  }
  ssize_t sum_of_part_numbers = 0;
  for (int i = 0; i < total_numbers; ++i) {
    if (parts_seen[i]) {
      sum_of_part_numbers += schematic_numbers[i];
    }
  }

  printf("%zd", sum_of_part_numbers);
  free(table);
  free(buffer);
  return 0;
}
