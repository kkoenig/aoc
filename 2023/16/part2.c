#include "cpt.h"
#include <stdlib.h>

typedef enum {
  UP = 0x01,
  DOWN = 0x2,
  LEFT = 0x4,
  RIGHT = 0x8,
} dir;

typedef struct {
  char c;
} cell;

typedef struct {
  cell **grid;
  int **activation_map;
  int rows, cols;
} facility;

int reflect(const dir d) {
  switch (d) {
  case LEFT:
    return DOWN;
  case RIGHT:
    return UP;
  case DOWN:
    return LEFT;
  case UP:
    return RIGHT;
  }
}
int flip(const dir d) {
  switch (d) {
  case LEFT:
    return RIGHT;
  case RIGHT:
    return LEFT;
  case DOWN:
    return UP;
  case UP:
    return DOWN;
  }
}

void cast_beam(facility *f, int r, int c, int direction) {
  while (r >= 0 && r < f->rows && c >= 0 && c < f->cols) {
    cell *current = &f->grid[r][c];
    if (f->activation_map[r][c] & direction) {
      break; // detect cycle
    }
    f->activation_map[r][c] |= direction;

    if (current->c == '|' && (direction == LEFT || direction == RIGHT)) {
      cast_beam(f, r - 1, c, UP);
      cast_beam(f, r + 1, c, DOWN);
      return;
    }
    if (current->c == '-' && (direction == UP || direction == DOWN)) {
      cast_beam(f, r, c - 1, LEFT);
      cast_beam(f, r, c + 1, RIGHT);
      return;
    }
    if (current->c == '/') {
      direction = reflect(direction);
    }

    if (current->c == '\\') {
      direction = flip(reflect(direction));
    }
    switch (direction) {
    case UP:
      --r;
      break;
    case DOWN:
      ++r;
      break;
    case LEFT:
      --c;
      break;
    case RIGHT:
      ++c;
      break;
    }
  }
}

int count(facility *f) {
  int energized = 0;
  for (int r = 0; r < f->rows; ++r) {
    for (int c = 0; c < f->cols; ++c) {
      energized += !!f->activation_map[r][c];
    }
  }
  return energized;
}

int main(void) {
  cpt_buffer2d input = cpt_slurp2d_stdin();

  const int cols = input.max_column_width;
  const int rows = input.num_rows;
  cell *data = calloc(1, rows * cols * sizeof(cell));
  cell **grid = calloc(1, input.num_rows * sizeof(cell *));
  int *activations_data = calloc(1, rows * cols * sizeof(int));
  int **activations = calloc(1, input.num_rows * sizeof(int *));
  for (size_t r = 0; r < input.num_rows; ++r) {
    grid[r] = data + r * cols;
    activations[r] = activations_data + r * cols;
    for (size_t c = 0; c < input.max_column_width; ++c) {
      grid[r][c] =
          (cell){.c = input.store.data[r * (input.max_column_width + 1) + c]};
    }
  }

  facility f = {
      .grid = grid,
      .activation_map = activations,
      .rows = rows,
      .cols = cols,
  };
  int max = 0;
  for (int r = 0; r < rows; ++r) {
    memset(activations_data, 0, rows * cols * sizeof(int));
    cast_beam(&f, r, 0, RIGHT);
    max = CPT_MAX(max, count(&f));
    memset(activations_data, 0, rows * cols * sizeof(int));
    cast_beam(&f, r, cols - 1, LEFT);
    max = CPT_MAX(max, count(&f));
  }
  for (int c = 0; c < cols; ++c) {
    memset(activations_data, 0, rows * cols * sizeof(int));
    cast_beam(&f, 0, c, DOWN);
    max = CPT_MAX(max, count(&f));
    memset(activations_data, 0, rows * cols * sizeof(int));
    cast_beam(&f, rows - 1, c, UP);
    max = CPT_MAX(max, count(&f));
  }
  printf("%d\n", max);

  free(activations_data);
  free(activations);
  free(grid);
  free(data);
  cpt_buffer2d_free(&input);
  return 0;
}
