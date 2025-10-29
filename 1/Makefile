DEBUG?=1

CC?=gcc

CC_FLAGS=-c -I./include -Wall -Werror
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


SOURCES=$(wildcard src/*.c)
OBJECTS=$(SOURCES:src/%.c=build/%.o)


build/%.o: src/%.c
	@mkdir -p build
	$(CC) $(CC_FLAGS) -o $@ $^


all: $(OBJECTS)
	$(CC) $(LD_FLAGS) -o ./build/main $^


clean:
	rm -rf build
