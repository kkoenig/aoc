#include "cpt.h"

int main(void) {
  cpt_buffer2d input = cpt_slurp2d_stdin();
  enum { FROM_L = 0, FROM_A, FROM_R, FROM_B };

  const int dx = 1;
  const int dy = input.max_column_width + 1;
  int jump[4][255] = {
      [FROM_B] = {['7'] = -dx, ['|'] = -dy, ['F'] = +dx},
      [FROM_A] = {['J'] = -dx, ['|'] = +dy, ['L'] = +dx},
      [FROM_L] = {['7'] = +dy, ['-'] = +dx, ['J'] = -dy},
      [FROM_R] = {['L'] = -dy, ['-'] = -dx, ['F'] = +dy},
  };
  int S = -1;
  for (size_t i = 0; i < input.store.size; ++i) {
    if (input.store.data[i] == 'S') {
      S = i;
      break;
    }
  }

  int max_x = input.max_column_width;
  int max_y = input.num_rows;
  int y = S / (input.max_column_width + 1);
  int x = S % (input.max_column_width + 1);
  const char *buffer = input.store.data;

  int prev = S;
  if (((y + 1) < max_y) && jump[FROM_B][(int)buffer[S + dy]]) {
    ++y;
  } else if (y > 1 && jump[FROM_A][(int)buffer[S - dy]]) {
    --y;
  } else if (x > 0 && jump[FROM_L][(int)buffer[S - dx]]) {
    --x;
  } else if ((x + 1) < max_x && jump[FROM_R][(int)buffer[S + dx]]) {
    ++x;
  }

  int curr = y * (input.max_column_width + 1) + x;
  int path_size = 1;
  while (curr != S) {
    int from;
    if (prev > curr) {
      from = (prev - curr) > 1 ? FROM_B : FROM_R;
    } else {
      from = (curr - prev) > 1 ? FROM_A : FROM_L;
    }
    prev = curr;
    curr += jump[from][(int)input.store.data[curr]];
    ++path_size;
  };

  printf("%d\n", path_size / 2);
  cpt_buffer2d_free(&input);
  return 0;
}
