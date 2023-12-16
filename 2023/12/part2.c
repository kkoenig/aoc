#include "cpt.h"

int count_arrangements(int max_size, int *ks, int num_k) {
  int total_ks = 0;
  for (int i = 0; i < num_k; ++i) {
    total_ks += ks[i];
  }
  int min_arrangement_size = total_ks + num_k - 1;
  if (min_arrangement_size > max_size) {
    return 0;
  }
  int balls_to_distribute = max_size - min_arrangement_size;
  int bins = num_k + 1;
  int n = balls_to_distribute + bins - 1;
  int k = bins - 1;

  int total = 1;
  for (int i = k; i < n; ++i) {
    total *= (i + 1);
  }
  for (int i = 1; i < (n - k); ++i) {
    total /= (i + 1);
  }
  return total;
}

typedef struct {
  const char *pattern;
  int pattern_size;
  const int *group;
  int num_groups;
} input;

typedef struct {
  int p_index;
  int g_index;
} state;

/*
int count_recursive(input i, state s) {
  int matches = 0;
  while (s.p_index < i.pattern_size) {
    switch (i.pattern[s.p_index]) {
    case '#':
      matches += count_recursive(i, s);
      break;
    case '?':
      // if we match start at 1 past the end, also keep going if we dont match
      matches += count_recursive(i, s);
      break;
    case '.':
      break;
    }
    ++s.p_index;
  }
  return matches;
}

int count(const char *pattern, int pattern_size, int *group, int num_groups) {
  return count_recursive(
      (input){
          .pattern = pattern,
          .pattern_size = pattern_size,
          .group = group,
          .num_groups = num_groups,
      },
      (state){
          .p_index = 0,
          .g_index = 0,
      });
}
*/

typedef struct {
  int *pattern;
  int size;
} split;

#include <assert.h>

int compile(const char *pattern, int pattern_size) {
  int *counts = alloca(sizeof(int) * pattern_size);
  int num_counts = 0;

  split *splits = alloca(sizeof(split) * pattern_size);
  int num_splits = 0;

  split *current_split = 0;
  int *current_count = 0;
  for (int i = 0; i < pattern_size; ++i) {
    if (pattern[i] != '.' && !current_split) {
      current_split = splits + num_splits++;
      current_split->pattern = counts + num_counts;
      current_split->size = 0;
    }

    switch (pattern[i]) {
    case '#':
      if (0 == current_count || *current_count < 0) {
        current_count = counts + num_counts++;
        *current_count = 0;
      }
      ++*current_count;
      break;
    case '?':
      if (0 == current_count || *current_count > 0) {
        current_count = counts + num_counts++;
        *current_count = 0;
      }
      --*current_count;
      break;
    case '.':
      if (current_split) {
        current_split->size = current_count - current_split->pattern + 1;
        current_split = 0;
        current_count = 0;
      }
      break;
    default:
      assert(false);
      return -1;
    }
  }
  if (current_split) {
    current_split->size = current_count - current_split->pattern + 1;
    current_split = 0;
    current_count = 0;
  }

  /*
  printf("pattern: %.*s\n", pattern_size, pattern);
  for (int i = 0; i < num_splits; ++i) {
    printf("split %d: ", i);
    for (int j = 0; j < splits[i].size; ++j) {
      printf("%d ", splits[i].pattern[j]);
    }
    printf("\n");
  }
  */
  return 1;
}

int main(void) {
#define S(X) X, strlen(X)
  assert(compile(S("##????##")));
  assert(compile(S("???.###")));
  assert(compile(S(".??..??...?##.")));
  /*
assert(1 == compile(S("?#?#?#?#?#?#?#?"), (int[]){1, 3, 1, 6}, 4));
assert(1 == compile(S("????.#...#..."), (int[]){4, 1, 1}, 3));
assert(4 == compile(S("????.######..#####."), (int[]){1, 6, 5}, 3));
assert(10 == compile(S("?###????????"), (int[]){3, 2, 1}, 3));
*/
  /*
  //   count_arrangements(S("???????"), (int[]){2, 1}, 2);
  assert(1 == count(S("##????##"), (int[]){3, 3}, 2));
  assert(1 == count(S("???.###"), (int[]){1, 1, 3}, 3));
  assert(4 == count(S(".??..??...?##."), (int[]){1, 1, 3}, 3));
  assert(1 == count(S("?#?#?#?#?#?#?#?"), (int[]){1, 3, 1, 6}, 4));
  assert(1 == count(S("????.#...#..."), (int[]){4, 1, 1}, 3));
  assert(4 == count(S("????.######..#####."), (int[]){1, 6, 5}, 3));
  assert(10 == count(S("?###????????"), (int[]){3, 2, 1}, 3));
  */
  return 0;
}
