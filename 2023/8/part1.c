#include "cpt.h"

static uint32_t hash(const char *buffer) {
  const int a = buffer[0] - 'A';
  const int b = buffer[1] - 'A';
  const int c = buffer[2] - 'A';
  return a * 26 * 26 + b * 26 + c;
}

int main(void) {
  cpt_buffer2d input = cpt_slurp2d_stdin();

#define MAX_ENCODING 17575 + 1
  int move_l[MAX_ENCODING], move_r[MAX_ENCODING];

  for (size_t i = 2; i < input.num_rows; ++i) {
    const uint32_t index = hash(input.rows[i].data);
    move_l[index] = hash(input.rows[i].data + 7);
    move_r[index] = hash(input.rows[i].data + 12);
  }
  int cur = hash("AAA");
  int end = hash("ZZZ");

  uint64_t i = 0;
  for (; cur != end; ++i) {
    const int op = input.rows[0].data[i % input.rows[0].size];
    cur = op == 'L' ? move_l[cur] : move_r[cur];
  }
  printf("%llu\n", i);
  cpt_buffer2d_free(&input);
  return 0;
}
