#include "assert.h"
#include "cpt.h"
#include "math.h"

int count_segment(const char *s, const int size, int starting_load) {
  int count = 0;

  int i = 0;
  int rounded_rocks = 0;
  while (i < size && s[i] != '#') {
    rounded_rocks += s[i] == 'O';
    ++i;
  }

  while (rounded_rocks-- > 0) {
    count += starting_load--;
  }

  return count;
}
int count_run(const char *s, const int size) {
  int total = 0;
  int i = 0;
  while (i < size) {
    int begin = i;
    while (i < size && s[i] != '#') {
      ++i;
    }
    total += count_segment(s + begin, i - begin, size - begin);
    while (i < size && s[i] == '#') {
      ++i;
    }
  }
  return total;
}

int main(void) {
  cpt_buffer2d input = cpt_slurp2d_stdin();

  char **inputs = malloc(input.max_column_width * sizeof(char *));
  for (size_t i = 0; i < input.max_column_width; ++i) {
    inputs[i] = malloc(sizeof(char) * input.num_rows);
  }
  for (size_t i = 0; i < input.max_column_width; ++i) {
    for (size_t j = 0; j < input.num_rows; ++j) {
      inputs[i][j] = input.store.data[j * (input.max_column_width + 1) + i];
    }
  }

  int total = 0;
  int num_inputs = input.max_column_width;
  int input_length = input.num_rows;
  for (int i = 0; i < num_inputs; ++i) {
    total += count_run(inputs[i], input_length);
  }
  printf("%d\n", total);

  cpt_buffer2d_free(&input);
  return 0;
}
