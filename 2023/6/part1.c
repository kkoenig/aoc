#include "cpt.h"

int main(void) {
  cpt_buffer2d input = cpt_slurp2d_stdin();

  cpt_cursor ct = cpt_cursor_ref(input.rows[0]);
  cpt_cursor_skip(&ct, strlen("Time: "));
  cpt_cursor cd = cpt_cursor_ref(input.rows[1]);
  cpt_cursor_skip(&cd, strlen("Distance: "));

  uint64_t product = 1;
  while (!cpt_cursor_eof(ct)) {
    const uint64_t time = cpt_next_u64(&ct);
    const uint64_t dist = cpt_next_u64(&cd);
    uint64_t num_ways = 0;
    for (uint64_t i = 0; i < time; ++i) {
      num_ways += (i * (time - i)) > dist;
    }
    if (num_ways) {
      product *= num_ways;
    }
  }
  printf("%llu\n", product);
  cpt_buffer2d_free(&input);
  return 0;
}
