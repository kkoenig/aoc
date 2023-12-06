#include "cpt.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  uint64_t begin;
  uint64_t size;
} range;
static uint64_t range_begin(const range v) { return v.begin; }
static uint64_t range_end(const range v) { return v.begin + v.size; }

typedef struct {
  uint64_t destination;
  range filter;
} mapping;
static bool range_intersects(const range a, const range b, range *out_result) {
  uint64_t s = CPT_MAX(range_begin(a), range_begin(b));
  uint64_t e = CPT_MIN(range_end(a), range_end(b));
  if (e <= s) {
    return false;
  }
  if (out_result) {
    *out_result = (range){.begin = s, .size = e - s};
  }
  return true;
}
static bool range_contains(const range a, const range b) {
  return range_begin(b) >= range_begin(a) && range_end(b) <= range_end(a);
}
static bool range_eq(const range a, const range b) {
  return a.begin == b.begin && a.size == b.size;
}

typedef struct {
  int num_ranges;
  range ranges[3];
} split_result;

static split_result split_range(const range target, const range filter) {
  range intersection;
  if (!range_intersects(target, filter, &intersection) ||
      range_eq(target, intersection)) { // subset does not need to be split
    return (split_result){0};
  }
  split_result result = {};
  // split target into 3 ranges
  const range a = {
      .begin = target.begin,
      .size = intersection.begin - target.begin,
  };
  if (a.size) {
    result.ranges[result.num_ranges++] = a;
  }
  const range b = {
      .begin = range_end(a),
      .size = intersection.size,
  };
  result.ranges[result.num_ranges++] = b;
  const range c = {
      .begin = range_end(b),
      .size =
          target.begin + target.size - intersection.begin - intersection.size,
  };
  if (c.size) {
    result.ranges[result.num_ranges++] = c;
  }
  return result;
}

int main(void) {
#define MAX_MAPPINGS 100
#define MAX_PIPELINE_INPUTS 1000
  int num_pipeline_inputs[8] = {0};
  range inputs[8][MAX_PIPELINE_INPUTS] = {0};

  cpt_buffer2d input = cpt_slurp2d_stdin();
  int current_row = 0;
  {
    // read seeds
    cpt_cursor c = cpt_cursor_ref(input.rows[current_row]);
    c.pos += strlen("seeds:");
    while (' ' == c.buffer.data[c.pos]) {
      inputs[0][num_pipeline_inputs[0]++] = (range){
          .begin = cpt_next_u64(&c),
          .size = cpt_next_u64(&c),
      };
    }
    current_row += 3; // skip to beginning of next pipeline stage
  }
  for (int k = 0; k < 7; ++k) {
    mapping *mappings = alloca(sizeof(mapping) * MAX_MAPPINGS);
    int num_mappings = 0;
    while (input.rows[current_row].size) {
      cpt_cursor c = cpt_cursor_ref(input.rows[current_row]);
      mappings[num_mappings++] = (mapping){
          .destination = cpt_next_u64(&c),
          .filter =
              (range){
                  .begin = cpt_next_u64(&c),
                  .size = cpt_next_u64(&c),
              },
      };
      ++current_row;
    }

    for (int i = 0; i < num_pipeline_inputs[k]; ++i) {
      const range target = inputs[k][i];
      // split ranges that intersect with mappings so all filtered ranges are
      // subsets
      for (int j = 0; j < num_mappings; ++j) {
        mapping m = mappings[j];
        split_result result = split_range(target, m.filter);
        if (!result.num_ranges) {
          continue;
        }
        inputs[k][i] = result.ranges[0];
        for (int z = 1; z < result.num_ranges; ++z) {
          inputs[k][num_pipeline_inputs[k]++] = result.ranges[z];
        }
        i = 0;
        break;
      }
    }
    // copy over all pipeline inputs (we will over write those matching filters)
    for (int i = 0; i < num_pipeline_inputs[k]; ++i) {
      inputs[k + 1][num_pipeline_inputs[k + 1]++] = inputs[k][i];
    }
    for (int i = 0; i < num_pipeline_inputs[k]; ++i) {
      const range target = inputs[k][i];
      for (int j = 0; j < num_mappings; ++j) {
        const mapping m = mappings[j];
        // because we split ranges (above) we can consider only subsets
        if (!range_contains(m.filter, target)) {
          continue;
        }
        inputs[k + 1][i] =
            (range){.begin = m.destination + target.begin - m.filter.begin,
                    .size = target.size};
        break;
      }
    }
    current_row += 2;
  }
  size_t min = SIZE_MAX;
  for (int i = 0; i < num_pipeline_inputs[7]; ++i) {
    min = CPT_MIN((size_t)inputs[7][i].begin, min);
  }
  printf("%zu\n", min);
  cpt_buffer2d_free(&input);

  return 0;
}
