#include "cpt.h"

enum {
  UP = 0x01,
  DOWN = 0x2,
  LEFT = 0x4,
  RIGHT = 0x8,
};

typedef struct {
  char c;
  uint32_t activation;
} cell;

typedef struct {
  cell **grid;
  int rows, cols;
} facility;

void cast_beam(facility *f, int r, int c, int direction) {
  while (r >= 0 && r < f->rows && c >= 0 && c < f->cols) {
    cell *current = &f->grid[r][c];
    if (current->activation & direction) {
      break; // detect cycle
    }
    current->activation |= direction;

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
      if (direction == LEFT) {
        direction = DOWN;
      } else if (direction == RIGHT) {
        direction = UP;
      } else if (direction == DOWN) {
        direction = LEFT;
      } else if (direction == UP) {
        direction = RIGHT;
      }
    }
    if (current->c == '\\') {
      if (direction == LEFT) {
        direction = UP;
      } else if (direction == RIGHT) {
        direction = DOWN;
      } else if (direction == DOWN) {
        direction = RIGHT;
      } else if (direction == UP) {
        direction = LEFT;
      }
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

void count_energized(facility *f) {
  int energized = 0;
  for (int r = 0; r < f->rows; ++r) {
    for (int c = 0; c < f->cols; ++c) {
      energized += !!f->grid[r][c].activation;
    }
  }
  printf("%d\n", energized);
}

int main(void) {
  cpt_buffer2d input = cpt_slurp2d_stdin();

  const int cols = input.max_column_width;
  const int rows = input.num_rows;
  cell *data = calloc(1, rows * cols * sizeof(cell));
  cell **grid = calloc(1, input.num_rows * sizeof(cell *));
  for (size_t r = 0; r < input.num_rows; ++r) {
    grid[r] = data + r * cols;
    for (size_t c = 0; c < input.max_column_width; ++c) {
      grid[r][c] =
          (cell){.c = input.store.data[r * (input.max_column_width + 1) + c]};
    }
  }

  facility f = {
      .grid = grid,
      .rows = rows,
      .cols = cols,
  };
  cast_beam(&f, 0, 0, RIGHT);
  count_energized(&f);

  free(data);
  free(grid);
  cpt_buffer2d_free(&input);
  return 0;
}
