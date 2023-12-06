#include "cpt.h"

int main(void) {
#define INPUTS_MAX 1000
  uint64_t inputs[INPUTS_MAX] = {0};
  int num_inputs = 0;

  cpt_buffer2d input = cpt_slurp2d_stdin();
  size_t r = 0;
  {
    // read seeds
    cpt_cursor c = cpt_cursor_ref(input.rows[r]);
    cpt_cursor_skip(&c, strlen("seeds:"));
    while (!cpt_cursor_eof(c)) {
      inputs[num_inputs++] = cpt_next_u64(&c);
    }
    r += 3; // skip to beginning of next pipeline stage
  }

  // Process each pipeline stage by:
  // - looping through each map and checking it against each input
  // - store inputs that have been mapped in place at the beginning of the array
  // - n^2 but inputs are small, could sort first to fix
  for (int k = 0; k < 7; ++k) {
    for (int modified = 0; r < input.num_rows && input.rows[r].size; ++r) {
      cpt_cursor c = cpt_cursor_ref(input.rows[r]);

      const int64_t result_begin = cpt_next_u64(&c);
      const int64_t filter_begin = cpt_next_u64(&c);
      const int64_t size = cpt_next_u64(&c);

      for (int i = modified; i < num_inputs; ++i) {
        const int64_t target = inputs[i];
        const int64_t delta = target - filter_begin;
        if (delta <= 0 || delta >= size) {
          continue;
        }

        inputs[i] = inputs[modified];
        inputs[modified++] = delta + result_begin;
      }
    }
    r += 2;
  }
  size_t min = SIZE_MAX;
  for (int i = 0; i < num_inputs; ++i) {
    min = CPT_MIN((size_t)inputs[i], min);
  }
  printf("%zu\n", min);
  cpt_buffer2d_free(&input);
  return 0;
}
