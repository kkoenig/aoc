#include "cpt.h"

typedef struct {
  int x, y;
} point;

int polygon_area(const point *points, const int num_points) {
  // https://en.wikipedia.org/wiki/Shoelace_formula
  int result = 0;
  for (int i = 0; i < num_points; ++i) {
    const point p1 = points[i];
    const point p2 = points[(i + 1) % num_points];
    result += p1.x * p2.y - p2.x * p1.y;
  }
  return abs(result) / 2;
}

int main(void) {
  int x = 0;
  int y = 0;
  point points[1000] = {0};
  int num_points = 0;
  int perimeter = 0;
  cpt_buffer2d input = cpt_slurp2d_stdin();
  for (size_t i = 0; i < input.num_rows; ++i) {
    cpt_buffer row = input.rows[i];
    const int direction = row.data[0];
    cpt_cursor c = cpt_cursor_ref(row);
    cpt_cursor_skip(&c, 2);
    const int v = cpt_read_u64(&c);
    perimeter += v;
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
  printf("%d\n",
         perimeter + polygon_area(points, num_points) - perimeter / 2 + 1);

  cpt_buffer2d_free(&input);
  return 0;
}
