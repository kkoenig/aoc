#include "cpt.h"

static uint32_t hash(const char *buffer) {
  const int a = buffer[0] - 'A';
  const int b = buffer[1] - 'A';
  const int c = buffer[2] - 'A';
  return a * 26 * 26 + b * 26 + c;
}

static bool is_terminal(const uint32_t k) { return (k % 26) == 'Z' - 'A'; }

static uint64_t gcd(uint64_t a, uint64_t b) {
  while (b != 0) {
    uint64_t temp = b;
    b = a % b;
    a = temp;
  }
  return a;
}

int main(void) {
  cpt_buffer2d input = cpt_slurp2d_stdin();

#define MAX_GHOSTS 100
  int ghosts[MAX_GHOSTS];
  int num_ghosts = 0;
#define MAX_ENCODING 17575 + 1
  int move_l[MAX_ENCODING], move_r[MAX_ENCODING];
  for (size_t i = 2; i < input.num_rows; ++i) {
    const uint32_t index = hash(input.rows[i].data);
    if (input.rows[i].data[2] == 'A') {
      ghosts[num_ghosts++] = index;
    }
    move_l[index] = hash(input.rows[i].data + 7);
    move_r[index] = hash(input.rows[i].data + 12);
  }

  uint64_t product = 1;
  for (int k = 0; k < num_ghosts; ++k) {
    uint64_t i = 0;
    for (; !is_terminal(ghosts[k]); ++i) {
      const int op = input.rows[0].data[i % input.rows[0].size];
      ghosts[k] = op == 'L' ? move_l[ghosts[k]] : move_r[ghosts[k]];
    }
    product *= i / gcd(i, product);
  }

  printf("%llu\n", product);
  cpt_buffer2d_free(&input);

  return 0;
}
