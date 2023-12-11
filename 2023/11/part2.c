#include "cpt.h"
#include "math.h"

int main(void) {
  cpt_buffer2d input = cpt_slurp2d_stdin();

#define MAX_ROWS 1000
  int64_t row_counts[MAX_ROWS] = {0};
  int64_t col_counts[MAX_ROWS] = {0};
  for (int64_t i = 0; i < (int64_t)input.num_rows; ++i) {
    for (int64_t j = 0; j < (int64_t)input.max_column_width; ++j) {
      if (input.store.data[i * (input.max_column_width + 1) + j] == '#') {
        ++row_counts[i];
        ++col_counts[j];
      }
    }
  }
  typedef struct {
    int64_t x, y;
  } point;
#define MAX_GALAXIES 1000
  point galaxies[MAX_GALAXIES] = {0};
  int64_t num_galaxies = 0;
  int64_t row = 0;
  for (int64_t i = 0; i < (int64_t)input.num_rows; ++i) {
    int col = 0;
    for (int64_t j = 0; j < (int64_t)input.max_column_width; ++j) {
      if (input.store.data[i * (input.max_column_width + 1) + j] == '#') {
        galaxies[num_galaxies++] = (point){
            .x = col + 1,
            .y = row + 1,
        };
      }
      col += (1000000 - 1) * !col_counts[j] + 1;
    }
    row += (1000000 - 1) * !row_counts[i] + 1;
  }
  int64_t sum = 0;
  for (int64_t i = 0; i < num_galaxies; ++i) {
    for (int64_t j = i + 1; j < num_galaxies; ++j) {
      point p1 = galaxies[i];
      point p2 = galaxies[j];
      sum += llabs(p1.x - p2.x) + llabs(p1.y - p2.y);
    }
  }

  printf("%lld\n", sum);
  cpt_buffer2d_free(&input);
  return 0;
}
