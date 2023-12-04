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
int is_part_marker(const char c) { return c != '.' && !isdigit(c) && c != 0; }
int main(void) {
  char *buffer = NULL;
  size_t buffer_size;
  ssize_t line_size;

  // Get rows,colls
  int rows = 2 /* padding */;
  int cols = 0;
  while (-1 != (line_size = getline(&buffer, &buffer_size, stdin))) {
    cols = line_size - 1 + 2 /* padding */;
    ++rows;
  };
  rewind(stdin);

  // Copy input data into array
  char *table = calloc(1, sizeof(char) * rows * cols);
  for (int r = 1; - 1 != getline(&buffer, &buffer_size, stdin); ++r) {
    for (int c = 1; c < cols - 1; ++c) {
      table[r * cols + c] = buffer[c - 1];
    }
  }
  free(buffer);
  rewind(stdin);

  ssize_t sum_of_part_numbers = 0;
  for (int r = 0; r < rows; ++r) {
    char *row = table + r * cols;
    for (int c = 0; c < cols;) {
      if (!isdigit(row[c])) {
        ++c;
        continue;
      }
      int b = c;
      int number = 0;
      while (c != cols && isdigit(row[c])) {
        number = number * 10 + row[c] - '0';
        ++c;
      }

      int is_part = 0;
      for (int y = r - 1; y <= r + 1; ++y) {
        for (int x = b - 1; x <= c; ++x) {
          is_part |= is_part_marker(table[y * cols + x]);
        }
      }
      sum_of_part_numbers += is_part * number;
    }
  }
  printf("%zd", sum_of_part_numbers);
  free(table);
  return 0;
}
