
CC     := clang
CFLAGS := -O3 -g -Wextra -Werror -Wall -fsanitize=undefined -fsanitize=integer

bin/2015/1: 2015/1/main.c
	@mkdir -p $(@D)
	$(CC) ./2015/$(@F)/main.c $(CFLAGS) -o $@

test: bin/2015/1
	diff <(./bin/2015/1 < ./2015/1/input) ./2015/1/expected
