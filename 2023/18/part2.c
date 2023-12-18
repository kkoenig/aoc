#include "cpt.h"

typedef struct {
  int64_t x, y;
} point;

int64_t polygon_area(const point *points, const int num_points) {
  // https://en.wikipedia.org/wiki/Shoelace_formula
  int64_t result = 0;
  for (int i = 0; i < num_points; ++i) {
    const point p1 = points[i];
    const point p2 = points[(i + 1) % num_points];
    result += p1.x * p2.y - p2.x * p1.y;
  }
  return llabs(result) / 2;
}

int main(void) {
  int64_t x = 0;
  int64_t y = 0;
  point points[1000] = {0};
  int num_points = 0;
  int64_t perimeter = 0;
  cpt_buffer2d input = cpt_slurp2d_stdin();
  for (size_t i = 0; i < input.num_rows; ++i) {
    cpt_buffer row = input.rows[i];
    int direction = row.data[row.size - 2] - '0';
    ((char *)row.data)[row.size - 2] = 0;
    cpt_cursor c = cpt_cursor_ref(row);
    cpt_cursor_skip(&c, 2);
    int u, v;
    sscanf(row.data + 2, "%d (#%x)", &u, &v);
    perimeter += v;
    switch (direction) {
    case 0:
      direction = 'R';
      break;
    case 1:
      direction = 'D';
      break;
    case 2:
      direction = 'L';
      break;
    case 3:
      direction = 'U';
      break;
    }
    switch (direction) {
    case 'L':
      x -= v;
      break;
    case 'R':
      x += v;
      break;
    case 'U':
      y -= v;
      break;
    case 'D':
      y += v;
      break;
    };
    points[num_points++] = (point){x, y};
  }
  // https://en.wikipedia.org/wiki/Pick%27s_theorem
  printf("%lld\n",
         perimeter + polygon_area(points, num_points) - perimeter / 2 + 1);

  cpt_buffer2d_free(&input);
  return 0;
}
