#include "cpt.h"

static uint64_t shift(uint64_t v) {
  uint64_t pow10[] = {1, 10, 100, 1000, 10000, 100000};
  uint64_t result = 0;
  while (v > 0) {
    v /= 10;
    ++result;
  }
  return pow10[result];
}

int main(void) {
  cpt_buffer2d input = cpt_slurp2d_stdin();

  cpt_cursor ct = cpt_cursor_ref(input.rows[0]);
  cpt_cursor_skip(&ct, strlen("Time: "));
  cpt_cursor cd = cpt_cursor_ref(input.rows[1]);
  cpt_cursor_skip(&cd, strlen("Distance: "));

  uint64_t time = 0;
  uint64_t dist = 0;
  while (!cpt_cursor_eof(ct)) {
    const uint64_t time_part = cpt_next_u64(&ct);
    const uint64_t dist_part = cpt_next_u64(&cd);
    time = time * shift(time_part) + time_part;
    dist = dist * shift(dist_part) + dist_part;
  };
  uint64_t num_ways = 0;
  for (uint64_t i = 0; i < time; ++i) {
    num_ways += (i * (time - i)) > dist;
  }
  printf("%llu\n", num_ways);
  cpt_buffer2d_free(&input);
  return 0;
}
