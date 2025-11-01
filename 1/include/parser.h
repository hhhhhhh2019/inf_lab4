#ifndef PARSER_H_
#define PARSER_H_

#include <stdio.h>
#include <stdlib.h>

#include "json.h"

enum ErrorType {
	E_SYNTAX,
};

typedef struct {
	enum ErrorType type;
	unsigned int start;
	unsigned int end;
} Error;

typedef struct {
	char* input;
	size_t input_len;
	size_t pos;

	char current;
	char lookahead;

	size_t errors_count;
	Error* errors;
} Parser;

static inline void parser_add_error(Parser* parser, Error error) {
	parser->errors =
		realloc(parser->errors, sizeof(Error) * (++parser->errors_count));
	parser->errors[parser->errors_count - 1] = error;
}

Node parse_JSON(Parser*);
static inline Parser parser_from_file(FILE* f) {
	size_t offset = ftell(f);

	fseek(f, 0, SEEK_END);
	size_t len = ftell(f) - offset;
	fseek(f, offset, SEEK_SET);

	char* buf = malloc(len);
	fread(buf, len, 1, f);

	return (Parser){
		.input = buf,
		.input_len = len,
		.pos = 0,
	};
}
void node_free(Node);
void node_to_bin(Node, FILE*);
#ifdef DEBUG
void print_node(Node, int offset);
#endif

#endif
