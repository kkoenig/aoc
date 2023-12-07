#include "cpt.h"

int char_to_index[255] = {
    ['A'] = 12, ['K'] = 11, ['Q'] = 10, ['J'] = 0, ['T'] = 9,
    ['9'] = 8,  ['8'] = 7,  ['7'] = 6,  ['6'] = 5, ['5'] = 4,
    ['4'] = 3,  ['3'] = 2,  ['2'] = 1,
};

typedef struct {
  int64_t power;
  int64_t bid;
} Hand;

int compare(const void *a, const void *b) {
  return ((Hand *)a)->power - ((Hand *)b)->power;
}

uint64_t category(const char *hand) {
  int counts[13] = {0};
  for (int i = 0; i < 5; ++i) {
    int index = char_to_index[(int)hand[i]];
    ++counts[index];
  }
  int max = 1;
  for (int i = 1; i < 13; ++i) {
    if (counts[i] > counts[max]) {
      max = i;
    }
  }
  counts[max] += counts[char_to_index[(int)'J']];
  int twos = 0;
  for (int i = 1; i < 13; ++i) {
    if (counts[i] == 2) {
      ++twos;
    }
  }
  switch (counts[max]) {
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
