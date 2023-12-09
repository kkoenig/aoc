#include "cpt.h"

int main(void) {
  cpt_buffer2d input = cpt_slurp2d_stdin();

  int64_t result = 0;
  for (size_t i = 0; i < input.num_rows; ++i) {
#define MAX_NUMBERS 100
    int64_t numbers[MAX_NUMBERS];
    int num_numbers = 0;
    cpt_cursor c = cpt_cursor_ref(input.rows[i]);
    while (!cpt_cursor_eof(c)) {
      numbers[num_numbers++] = cpt_next_i64(&c);
    }

    int64_t tails[MAX_NUMBERS];
    int32_t num_tails = 0;

    int64_t round_sum = 1;
    tails[num_tails++] = numbers[0];
    while (round_sum) {
      round_sum = 0;
      for (int j = 1; j < num_numbers; ++j) {
        numbers[j - 1] = numbers[j] - numbers[j - 1];
        round_sum += numbers[j - 1];
      }
      tails[num_tails++] = numbers[0];
      --num_numbers;
    }
    int64_t n = 0;
    for (int j = num_tails - 1; j >= 0; --j) {
      n = tails[j] - n;
    }
    result += n;
  }
  printf("%lld\n", result);
  cpt_buffer2d_free(&input);
  return 0;
}
