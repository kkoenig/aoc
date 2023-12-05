#include "cpt.h"
int main(void) {
  const int skip[255] = {
      ['r'] = sizeof("red"), ['g'] = sizeof("green"), ['b'] = sizeof("blue")};
  const int limits[255] = {['r'] = 12, ['g'] = 13, ['b'] = 14};

  __attribute__((__cleanup__(cpt_buffer2d_free))) cpt_buffer2d input =
      cpt_slurp2d_stdin();
  ssize_t total = input.num_rows * (input.num_rows + 1) / 2;
  for (size_t r = 0; r < input.num_rows; ++r) {
    cpt_cursor c = cpt_cursor_ref(input.rows[r]);
    cpt_cursor_read_until(&c, ':');
    cpt_cursor_skip(&c, 1); // skip ':'
    do {
      int number = cpt_next_u32(&c);
      cpt_cursor_skip(&c, 1); // skip ' '
      const int index = cpt_cursor_peek(&c);
      cpt_cursor_skip(&c, skip[index] + 1); // skip past comma
      if (number > limits[index]) {
        total -= r + 1;
        break;
      }
    } while (!cpt_cursor_eof(c));
  }
  printf("%zd", total);
  return 0;
}
