#include "parser.h"
#include "json.h"

static void parser_next(Parser*);
static void parse_ws(Parser*);
static Node parse_value(Parser*);
// static Node parse_array(Parser*);
// static Node parse_object(Parser*);

static void parser_next(Parser* parser) {
	parser->current = parser->lookahead;
	parser->lookahead = parser->input.p[parser->pos++ + 1];
}

static void parse_ws(Parser* parser) {
loop:
	switch (parser->current) {
	case 0x20:
	case 0x09:
	case 0x0a:
	case 0x0d:
		parser_next(parser);
		goto loop;
	default:
		return;
	}
}

static Node parse_value(Parser* parser) {
	unsigned int start = parser->pos;
	switch (parser->current) {
	case 'f':
		goto lfalse;
	case 'n':
		goto lnull;
	case 't':
		goto ltrue;
	default:
		goto error;
	}

lfalse:
	parser_next(parser);
	if (parser->current != 'a')
		goto error;
	parser_next(parser);
	if (parser->current != 'l')
		goto error;
	parser_next(parser);
	if (parser->current != 's')
		goto error;
	parser_next(parser);
	if (parser->current != 'e')
		goto error;
	parser_next(parser);
	return (Node){
		.type = N_BOOL,
		.start = start,
		.end = parser->pos,
		._bool = 0,
	};
ltrue:
	parser_next(parser);
	if (parser->current != 'r')
		goto error;
	parser_next(parser);
	if (parser->current != 'u')
		goto error;
	parser_next(parser);
	if (parser->current != 'e')
		goto error;
	parser_next(parser);
	return (Node){
		.type = N_BOOL,
		.start = start,
		.end = parser->pos,
		._bool = 1,
	};
lnull:
	parser_next(parser);
	if (parser->current != 'u')
		goto error;
	parser_next(parser);
	if (parser->current != 'l')
		goto error;
	parser_next(parser);
	if (parser->current != 'l')
		goto error;
	parser_next(parser);
	return (Node){
		.type = N_NULL,
		.start = start,
		.end = parser->pos,
	};
error:
	parser_next(parser);
	return (Node){
		.type = N_ERROR,
		.start = start,
		.end = parser->pos,
		.error = E_SYNTAX,
	};
}

Node parse_JSON(Parser* parser) {
	parser->current = parser->input.p[parser->pos];
	parser->lookahead = parser->input.p[parser->pos + 1];
	parse_ws(parser);
	return parse_value(parser);
}
