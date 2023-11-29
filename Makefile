CC     := clang
CFLAGS := -O3 -g -Wextra -Werror -Wall -fsanitize=undefined -fsanitize=integer

bin/%: %.c
	@mkdir -p $(@D)
	$(CC) $< $(CFLAGS) -o $@

BINS := bin/2015/1/part1 bin/2015/1/part2 bin/2015/2/part1
build: $(BINS)

test: $(BINS)
	diff <(bin/2015/1/part1 < ./2015/1/input) ./2015/1/expected1
	diff <(bin/2015/1/part2 < ./2015/1/input) ./2015/1/expected2
	diff <(bin/2015/2/part1 < ./2015/2/input) ./2015/2/expected1

clean:
	rm -rf ./bin
