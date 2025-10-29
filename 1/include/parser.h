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


Node parseJSON(Parser*);


#endif
