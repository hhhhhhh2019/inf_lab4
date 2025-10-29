#ifndef PARSER_H_
#define PARSER_H_

#include <stdio.h>

#include "json.h"
#include "str.h"

typedef struct {
	Str input;
	size_t pos;

	char current;
	char lookahead;
} Parser;

Node parse_JSON(Parser*);
static inline Parser parser_new(Str str) {
	return (Parser){.input = str, .pos = 0};
}
#ifdef DEBUG
void print_node(Node, int offset);
#endif

#endif
