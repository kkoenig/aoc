#include "assert.h"
#include "cpt.h"
#include "math.h"

static uint32_t expand_wildcards(const uint32_t wild_mask, const int size,
                                 int seed) {
  assert(size < 32);
  uint32_t result = 0;
  for (int i = __builtin_ctz(wild_mask); i < size; ++i) {
    if ((1u << i) & wild_mask) {
      result = (result << 1u) | (seed & 1);
      seed >>= 1;
    } else {
      result = (result << 1u);
    }
  }
  return result;
}

static uint32_t nbits(uint32_t k) {
  if (k >= 32) {
    return UINT32_MAX;
  }
  return (1u << k) - 1;
}

static bool match_mask_all(const int set_bits_remaining, const uint32_t source,
                           const int source_size, const int k[],
                           const int num_k) {
  if (__builtin_popcount(source) != set_bits_remaining) {
    return false;
  }
  if (num_k == 0) {
    return source == 0;
  }
  const uint32_t bits = nbits(k[0]);
  for (int i = __builtin_ctz(source); i < (int)(1 + source_size - k[0]); ++i) {
    const uint32_t mask = nbits(1 + i + k[0]);
    const uint32_t a = source & mask;
    const uint32_t b = (bits << i) & mask;
    if (a == b) {
      // once we find our first match the remainder must match
      return match_mask_all(set_bits_remaining - k[0], source >> (i + k[0] + 1),
                            source_size - (i + k[0] + 1), k + 1, num_k - 1);
    }
  }
  return false;
}

static int matches(const char *s, const int size, const int k[],
                   const int num_k) {
  assert(size < 32);
  uint32_t fixed_mask = 0;
  uint32_t wild_mask = 0;
  for (int i = 0; i < size; ++i) {
    fixed_mask = (fixed_mask << 1) | (s[size - i - 1] == '#');
    wild_mask = (wild_mask << 1) | (s[i] == '?');
  }
  int num_wild = __builtin_popcount(wild_mask);
  int num_fixed = __builtin_popcount(fixed_mask);
  assert(num_wild < 32);
  int set_bits_remaining = 0;
  for (int i = 0; i < num_k; ++i) {
    set_bits_remaining += k[i];
  }
  const int num_wild_needed = set_bits_remaining - num_fixed;
  int total_matches = 0;
  for (uint32_t i = nbits(num_wild_needed); i < (1u << num_wild); ++i) {
    if (__builtin_popcount(i) != num_wild_needed) {
      continue;
    }
    const uint32_t s = fixed_mask | expand_wildcards(wild_mask, size, i);
    total_matches += match_mask_all(set_bits_remaining, s, size, k, num_k);
  }
  return total_matches;
}

int main(void) {
#define S(X) X, strlen(X)
  assert(1 == matches(S("##????##"), (int[]){3, 3}, 2));
  assert(1 == matches(S("???.###"), (int[]){1, 1, 3}, 3));
  assert(4 == matches(S(".??..??...?##."), (int[]){1, 1, 3}, 3));
  assert(1 == matches(S("?#?#?#?#?#?#?#?"), (int[]){1, 3, 1, 6}, 4));
  assert(1 == matches(S("????.#...#..."), (int[]){4, 1, 1}, 3));
  assert(4 == matches(S("????.######..#####."), (int[]){1, 6, 5}, 3));
  assert(10 == matches(S("?###????????"), (int[]){3, 2, 1}, 3));

  uint64_t total = 0;
  cpt_buffer2d buffer = cpt_slurp2d_stdin();
  for (size_t i = 0; i < buffer.num_rows; ++i) {
    cpt_buffer row = buffer.rows[i];
    int j = 0;
    while (row.data[j] != ' ') {
      ++j;
    }
    cpt_cursor c = cpt_cursor_ref(row);
    cpt_cursor_skip(&c, j + 1);

#define MAX_SPLITS 100
    int k[MAX_SPLITS];
    int num_k = 0;
    while (!cpt_cursor_eof(c)) {
      k[num_k++] = cpt_next_u64(&c);
      cpt_cursor_skip(&c, 1);
    }

    total += matches(row.data, j, k, num_k);
  }
  printf("%llu\n", total);

  cpt_buffer2d_free(&buffer);
  return 0;
}
