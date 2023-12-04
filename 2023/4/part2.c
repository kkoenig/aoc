#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
  char *buffer = NULL;
  size_t buffer_size;
  ssize_t line_size;

#define MAX_GAMES 1000
  int *winners_per_game = calloc(1, MAX_GAMES);
  int *totals = calloc(1, MAX_GAMES);

  int n, lines = 0;
  while (-1 != (line_size = getline(&buffer, &buffer_size, stdin))) {
#define MAX_WINNING_NUMBERS 1000
    int winning_numbers[MAX_WINNING_NUMBERS] = {0};

    int bytes_read, i = 0;
    sscanf(i + buffer, "Card%d:%n", &n, &bytes_read);
    i += bytes_read;

    while (0 < sscanf(i + buffer, "%d%n", &n, &bytes_read)) {
      winning_numbers[n] = 1;
      i += bytes_read;
    }
    while ('|' != buffer[i]) {
      ++i;
    }
    ++i;

    int num_winning_tickets = 0;
    while (0 < sscanf(i + buffer, "%d%n", &n, &bytes_read)) {
      num_winning_tickets += !!winning_numbers[n];
      i += bytes_read;
    }
    winners_per_game[lines++] = num_winning_tickets;
  }
  free(buffer);

  for (int i = 0; i < lines; ++i) {
    totals[i] = 1;
  }

  for (int i = 0; i < lines; ++i) {
    for (int j = i + 1; j < lines && j < i + 1 + winners_per_game[i]; ++j) {
      totals[j] += totals[i];
    }
  }
  int total = 0;
  for (int i = 0; i < lines; ++i) {
    total += totals[i];
  }

  free(totals);
  free(winners_per_game);
  printf("%u", total);
  return 0;
}
