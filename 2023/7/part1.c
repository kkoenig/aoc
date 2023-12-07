#include "cpt.h"

int char_to_index[255] = {
    ['A'] = 12, ['K'] = 11, ['Q'] = 10, ['J'] = 9, ['T'] = 8,
    ['9'] = 7,  ['8'] = 6,  ['7'] = 5,  ['6'] = 4, ['5'] = 3,
    ['4'] = 2,  ['3'] = 1,  ['2'] = 0,
};

typedef struct {
  int64_t power;
  int64_t bid;
} Hand;

int compare(const void *a, const void *b) {
  return ((Hand *)a)->power - ((Hand *)b)->power;
}
uint64_t category(const char *hand) {
  int max = 0;
  int counts[13] = {0};
  for (int i = 0; i < 5; ++i) {
    int index = char_to_index[(int)hand[i]];
    ++counts[index];
    max = CPT_MAX(counts[index], max);
  }
  int twos = 0;
  for (int i = 0; i < 13; ++i) {
    if (counts[i] == 2) {
      ++twos;
    }
  }
  switch (max) {
  case 5:
    return 6;
  case 4:
    return 5;
  case 3:
    return 3 + twos;
  case 2:
    return 1 + twos - 1;
  default:
    return 0;
  }
};

int main(void) {
  cpt_buffer2d input = cpt_slurp2d_stdin();

  Hand hands[1000];
  for (size_t i = 0; i < input.num_rows; ++i) {
    cpt_cursor c = cpt_cursor_ref(input.rows[i]);
    cpt_cursor_skip(&c, 5);

    const char *h = c.buffer.data;
    int n = category(h);
    for (int j = 0; j < 5; ++j) {
      n = n * 13 + char_to_index[(int)h[j]];
    }
    hands[i] = (Hand){.bid = cpt_next_u64(&c), .power = n};
  }
  qsort(hands, input.num_rows, sizeof(Hand), compare);
  uint64_t total = 0;
  for (size_t i = 0; i < input.num_rows; ++i) {
    total += hands[i].bid * (i + 1);
  }
  printf("%llu\n", total);
  cpt_buffer2d_free(&input);

  return 0;
}
