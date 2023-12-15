#include "cpt.h"

int main(void) {
  cpt_buffer input = cpt_slurp_stdin();
  int s = 0;
  for (size_t i = 0; i < input.size; ++i) {
    int v = 0;
    for (; i < input.size - 1 && input.data[i] != ','; ++i) {
      v += input.data[i];
      v *= 17;
      v = v % 256;
    }
    s += v;
  }
  printf("%d\n", s);
  cpt_buffer_free(&input);
  return 0;
}
