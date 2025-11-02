DEBUG?=1

CC?=gcc

CC_FLAGS=-c -I./include -Wall -Werror -MMD -MP
LD_FLAGS=-lm -Wall -Werror

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


.PHONY: all clean

%/:
	@mkdir -p $@

$(BUILD)/%.o: src/%.c | $(BUILD)/
	$(CC) $(CC_FLAGS) -o $@ $<

build: $(OBJ)
	$(CC) $(LD_FLAGS) -o $(BUILD)/main $^

all: build

clean:
	rm -r $(BUILD)

-include $(DEP)

tests/%.in: tests/%.out
	@diff -q <(build/main $@ 2>/dev/null) <(xxd -r $<) || \
	(echo "test $@ failed"; exit 1) && \
	(echo "test $@ passed")

test: build $(TESTS)
