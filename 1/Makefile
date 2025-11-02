SHELL := /bin/bash
MAKEFLAGS += -k

OUT=json2bin

DEBUG?=0

CC?=gcc

CC_FLAGS=-c -I./include -Wall -MMD -MP
LD_FLAGS=-lm -Wall

ifeq ($(DEBUG), 1)
	CC_FLAGS+=-ggdb -Og -DDEBUG
	LD_FLAGS+=-ggdb -Og -DDEBUG

	CC_FLAGS+=-fsanitize=address
	LD_FLAGS+=-fsanitize=address

	# CC_FLAGS+=-fsanitize=thread
	# LD_FLAGS+=-fsanitize=thread

	CC_FLAGS+=-fsanitize=leak
	LD_FLAGS+=-fsanitize=leak

	CC_FLAGS+=-fsanitize=undefined
	LD_FLAGS+=-fsanitize=undefined
endif


SRC=$(wildcard src/*.c)
BUILD=build
OBJ=$(SRC:src/%.c=$(BUILD)/%.o)
DEP=$(OBJ:.o=.d)

TESTS=$(wildcard tests/*.in)


.PHONY: all clean test

%/:
	@mkdir -p $@

$(BUILD)/%.o: src/%.c | $(BUILD)/
	$(CC) $(CC_FLAGS) -o $@ $<

build: $(OBJ)
	$(CC) --version
	$(CC) $(LD_FLAGS) -o "$(BUILD)/$(OUT)" $^

all: build

clean:
	rm -r "$(BUILD)"

-include $(DEP)

FORCE: ;

tests/%.in: tests/%.out FORCE
	@diff -q <("$(BUILD)/$(OUT)" $@ 2>/dev/null) <(xxd -r $<) >/dev/null || \
	(echo "test $@ failed"; \
	 "$(BUILD)/$(OUT)" $@ 2>/dev/null | xxd; \
	 xxd -r $< | xxd; false) && \
	(echo "test $@ passed")

test: build $(TESTS)
