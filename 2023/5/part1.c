#include "cpt.h"
#include <stdio.h>
#include <stdlib.h>

int main(void) {
#define INPUTS_MAX 1000
  uint64_t inputs[8][INPUTS_MAX] = {0};
  int num_inputs = 0;

  cpt_buffer2d input = cpt_slurp2d_stdin();
  int current_row = 0;
  {
    // read seeds
    cpt_cursor c = cpt_cursor_ref(input.rows[current_row]);
    c.pos += strlen("seeds:");
    while (' ' == c.buffer.data[c.pos]) {
      inputs[0][num_inputs++] = cpt_next_u64(&c);
    }
    current_row += 3; // skip to beginning of next pipeline stage
  }
  for (int k = 0; k < 7; ++k) {
    for (int i = 0; i < num_inputs; ++i) {
      inputs[k + 1][i] = inputs[k][i];
    }
    while (input.rows[current_row].size) {
      cpt_cursor c = cpt_cursor_ref(input.rows[current_row]);

      const uint64_t destination_range_start = cpt_next_u64(&c);
      const uint64_t source_range_start = cpt_next_u64(&c);
      const uint64_t range_size = cpt_next_u64(&c);

      for (int i = 0; i < num_inputs; ++i) {
        uint64_t source = inputs[k][i];
        if (source_range_start > source) {
          continue;
        }

        uint64_t relative = source - source_range_start;
        if (relative < range_size) {
          inputs[k + 1][i] = relative + destination_range_start;
        }
      }
      ++current_row;
    }
    current_row += 2;
  }
  size_t min = SIZE_MAX;
  for (int i = 0; i < num_inputs; ++i) {
    min = CPT_MIN((size_t)inputs[7][i], min);
  }
  printf("%zu\n", min);
  cpt_buffer2d_free(&input);
  return 0;
}
