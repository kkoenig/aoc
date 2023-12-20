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
  int *nodes;
  int size;
  int max_length;
} split;

typedef struct {
  const split *splits;
  int num_splits;
  const int *groups;
  int num_groups;
} input;

typedef struct {
  int s_index; // index into split
  int n_index; // index into split node
  int g_index; // index into groups
} state;

// pattern: ##????##
// 0: 2 -4 2

// ##???##
// match 3?

int greedy_match(const split *s, const int n_index, int group_size) {
  if (n_index >= s->size) {
    return 0;
  }

  int i = n_index;
  for (; group_size > 0 && i < s->size; ++i) {
    int node = s->nodes[i];
    if (node > group_size) {
      return 0;
    }
    if (node < 0) {
      --group_size;
    } else {
      group_size -= node;
    }
  }
  // the very next element is a fixed block
  if (i < s->size && s->nodes[i] > 0) {
    return 0;
  }

  return group_size == 0;
}

int count_matches(input i, state s) {
  if (s.g_index >= i.num_groups) {
    return 0; // todo: check other index?
  }
  int total = 0;
  while (s.s_index < i.num_splits) {
    const split *split = i.splits + s.s_index;
    while (s.n_index < split->size) {
      if (!greedy_match(i.splits, s.n_index, i.groups[s.g_index])) {
        return 0;
      }
      const int node = split->nodes[s.n_index];
      const int sub_matches = count_matches(i, (state){
                                                   .g_index = s.g_index + 1,
                                               });
      total += sub_matches;
      if (node > 0) {
        return total;
      }
      ++s.n_index;
    }
    s.n_index = 0;
    ++s.s_index;
  }
  return total;
}

#include <assert.h>

int compile(const char *pattern, int pattern_size, const int *groups,
            const int num_groups) {
  int *counts = alloca(sizeof(int) * pattern_size);
  int num_counts = 0;

  split *splits = alloca(sizeof(split) * pattern_size);
  int num_splits = 0;

  split *current_split = 0;
  int *current_count = 0;
  for (int i = 0; i <= pattern_size; ++i) {
    const int v = i < pattern_size ? pattern[i] : '.';
    if (v != '.' && !current_split) {
      current_split = splits + num_splits++;
      *current_split = (split){
          .nodes = counts + num_counts,
      };
    }

    switch (v) {
    case '#':
      if (0 == current_count || *current_count < 0) {
        current_count = counts + num_counts++;
        *current_count = 0;
      }
      ++current_split->max_length;
      ++*current_count;
      break;
    case '?':
      if (0 == current_count || *current_count) {
        current_count = counts + num_counts++;
        *current_count = 0;
      }
      ++current_split->max_length;
      --*current_count;
      break;
    case '.':
      if (current_split) {
        current_split->size = current_count - current_split->nodes + 1;
        current_split = 0;
        current_count = 0;
      }
      break;
    default:
      assert(false);
      return -1;
    }
  }

  // int** prefix_match_table = calloc
  // matches[g][i]
  // matches(g, node_index)

  /*
  int *matches_data = alloca(sizeof(int) * num_groups * num_counts);
  int **matches = alloca(sizeof(int *) * num_groups);
  for (int i = 0; i < num_groups; ++i) {
    matches[i] = matches_data + num_counts;
  }
  for (int i = 0; i < num_splits; ++i) {
    for (int j = 0; j < splits[i].size; ++j) {
      for (int g = 0; g < num_groups; ++g) {
        int m = greedy_match(&splits[i], k, groups[g]);
      }
    }
  }
  */

  //   | 2 | ? | ? | ? | ? | 2
  //   | 0 | 1 | 2 | 3 | 4 | 5
  // 3 |
  printf("pattern: %.*s\n", pattern_size, pattern);
  for (int i = 0; i < num_splits; ++i) {
    printf(" %d (%d):\n   ", i, splits[i].max_length);
    for (int j = 0; j < splits[i].size; ++j) {
      if (splits[i].nodes[j] < 0) {
        printf("?  ");
      } else {
        printf("%d  ", splits[i].nodes[j]);
      }
    }
    printf("\n");
    /*
    printf("   ");
    for (int j = 0; j < splits[i].size; ++j) {
      printf("%d  ", j);
    }
    printf("\n");
    */
    for (int g = 0; g < num_groups; ++g) {
      printf("%d  ", groups[g]);
      for (int k = 0; k < splits[i].size; ++k) {
        int m = greedy_match(&splits[i], k, groups[g]);
        printf("%d  ", m);
      }
      printf("\n");
    }
  }

  count_matches(
      (input){
          .splits = splits,
          .num_splits = num_splits,
          .groups = groups,
          .num_groups = num_groups,
      },
      (state){0});
  return 1;
}

int main(void) {
#define S(X) X, strlen(X)
  // printf("%d\n", compile(S("???????"), (int[]){2, 1}, 2));
  /*
assert(compile(S("##????##")));
assert(compile(S("???.###")));
assert(compile(S(".??..??...?##.")));
assert(compile(S("?#?#?#?#?#?#?#?")));
assert(compile(S("????.#...#...")));
assert(compile(S("????.######..#####.")));
assert(compile(S("?###????????")));
*/
  //   count_arrangements(S("???????"), (int[]){2, 1}, 2);
  // assert(1 == compile(S("???????.#"), (int[]){1, 1, 1}, 3));
  /*
assert(1 == compile(S("???.###????.###????.###????.###????.###"),
                    (int[]){1, 1, 3, 1, 1, 3, 1, 1, 3, 1, 1, 3, 1, 1, 3},
                    15));
  assert(1 == compile(S("##????##"), (int[]){3, 3}, 2));
  assert(1 == compile(S("???.###"), (int[]){1, 1, 3}, 3));
  assert(4 == compile(S(".??..??...?##."), (int[]){1, 1, 3}, 3));
  assert(1 == compile(S("?#?#?#?#?#?#?#?"), (int[]){1, 3, 1, 6}, 4));
  assert(1 == compile(S("????.#...#..."), (int[]){4, 1, 1}, 3));
  assert(4 == compile(S("????.######..#####."), (int[]){1, 6, 5}, 3));
  assert(10 == compile(S("?###????????"), (int[]){3, 2, 1}, 3));
*/
  return 0;
}
