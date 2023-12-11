#include "cpt.h"
#include "math.h"

int main(void) {
  cpt_buffer2d input = cpt_slurp2d_stdin();

  int row_counts[1000] = {0};
  int col_counts[1000] = {0};
  for (int i = 0; i < (int)input.num_rows; ++i) {
    for (int j = 0; j < (int)input.max_column_width; ++j) {
      if (input.store.data[i * (input.max_column_width + 1) + j] == '#') {
        ++row_counts[i];
        ++col_counts[j];
      }
    }
  }
  typedef struct {
    int x, y;
  } point;
  point galaxies[1000] = {0};
  int num_galaxies = 0;
  int row = 0;
  int col = 0;
  for (int i = 0; i < (int)input.num_rows; ++i) {
    col = 0;
    for (int j = 0; j < (int)input.max_column_width; ++j) {
      if (input.store.data[i * (input.max_column_width + 1) + j] == '#') {
        galaxies[num_galaxies++] = (point){
            .x = col + 1,
            .y = row + 1,
        };
      }
      col += !col_counts[j] + 1;
    }
    row += !row_counts[i] + 1;
  }
  int sum = 0;
  for (int i = 0; i < num_galaxies; ++i) {
    for (int j = i + 1; j < num_galaxies; ++j) {
      point p1 = galaxies[i];
      point p2 = galaxies[j];
      sum += abs(p1.x - p2.x) + abs(p1.y - p2.y);
    }
  }

  printf("%d\n", sum);
  cpt_buffer2d_free(&input);
  return 0;
}
