#include "assert.h"
#include "cpt.h"
#include "ctype.h"
#include "math.h"

int hash(const char *s, const int size) {
  int v = 0;
  for (int i = 0; i < size; ++i) {
    v += s[i];
    v *= 17;
    v = v % 256;
  }

  return v;
}

typedef struct {
  const char *label;
  int label_size;
  int focal_length;
} lens;
typedef struct {
#define MAX_LENSES 10
  lens lenses[10];
  int num_lenses;
} box;

static void add(box *box, const char *label, const int label_size,
                uint32_t value) {
  assert(box->num_lenses < MAX_LENSES);

  for (int i = 0; i < box->num_lenses; ++i) {
    lens *l = box->lenses + i;
    if (l->label_size == label_size && !strncmp(l->label, label, label_size)) {
      box->lenses[i].focal_length = value;
      return;
    }
  }
  box->lenses[box->num_lenses++] = (lens){
      .label = label,
      .label_size = label_size,
      .focal_length = value,
  };
}

static void rm(box *box, const char *label, const int label_size) {
  int i = 0;
  for (; i < box->num_lenses; ++i) {
    lens *l = box->lenses + i;
    if (l->label_size == label_size && !strncmp(l->label, label, label_size)) {
      break;
    }
  }
  if (i == box->num_lenses) {
    return;
  }

  for (; i < box->num_lenses - 1; ++i) {
    box->lenses[i] = box->lenses[i + 1];
  }
  --box->num_lenses;
}

int main(void) {
  cpt_buffer input = cpt_slurp_stdin();

  box boxes[256] = {0};

  for (size_t i = 0; i < input.size;) {
    while (i < input.size && isspace(input.data[i])) {
      ++i;
    }
    size_t j = i;
    while (j < input.size && input.data[j] != ',' && !isspace(input.data[j])) {
      ++j;
    }
    size_t k = i;
    while (k < j && input.data[k] != '=' && input.data[k] != '-') {
      ++k;
    }
    const char *label = input.data + i;
    const int label_size = k - i;
    const int op = input.data[k];
    const int box_index = hash(label, label_size);
    if ('=' == op) {
      const int lv = *(input.data + k + 1) - '0';
      add(boxes + box_index, label, label_size, lv);
    } else {
      assert(op == '-');
      rm(boxes + box_index, label, label_size);
    }

    i = j + 1;
  }
  uint64_t total = 0;
  for (size_t i = 0; i < 256; ++i) {
    box *b = boxes + i;
    if (!b->num_lenses) {
      continue;
    }
    for (int j = 0; j < b->num_lenses; ++j) {
      total += (i + 1) * (j + 1) * b->lenses[j].focal_length;
    }
  }
  printf("%llu\n", total);
  cpt_buffer_free(&input);
  return 0;
}
