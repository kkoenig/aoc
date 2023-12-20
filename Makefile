SHELL  := bash
CC     := clang
CFLAGS := -O3 -g3 \
		  -std=c11\
		  -fno-omit-frame-pointer \
		  -I./deps \
		  -Wextra \
		  -Werror \
		  -Wall \
		  -Wdouble-promotion \
		  -Wno-unused-parameter \
		  -Wno-unused-function \
		  -fsanitize=undefined \
		  -fsanitize=integer \
		  -fsanitize=address

PARTS   := $(basename $(wildcard */*/*.c))
TESTS   := $(addprefix test-, $(subst part,,$(subst /,-,$(PARTS))))
TARGETS := $(addprefix bin/, $(PARTS))

build: $(TARGETS)

test: $(TESTS)

bin/%: %.c deps/cpt.h Makefile
	@mkdir -p $(@D)
	$(CC) $< $(CFLAGS) -o $@

year     = $(word 1, $(subst -, ,$1))
day      = $(word 2, $(subst -, ,$1))
yearday  = $(call year,$1)/$(call day, $1)
input    = ./$(call yearday, $1)/input
expected = ./$(call yearday, $1)/expected

.SECONDEXPANSION:
test-%-1: ./bin/$$(call yearday, %)/part1
	diff <($< < $(call input, $*)) $(call expected, $*)1

test-%-2: ./bin/$$(call yearday, %)/part2
	diff <($< < $(call input, $*)) $(call expected, $*)2

clean:
	rm -rf ./bin

deps/cpt.h:
	@mkdir -p $(@D)
	curl -sLo $@ https://raw.githubusercontent.com/kkoenig/cpt/e038f7c0b88cdf5f09b4752bdcfe920d7b0b14d3/cpt.h

.PHONY: build test clean
