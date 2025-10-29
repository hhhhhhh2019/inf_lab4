#include "parser.h"
#include "json.h"

static void parserNext(Parser*);
static void parseWs(Parser*);
static Node parseValue(Parser*);
// static Node parseArray(Parser*);
// static Node parseObject(Parser*);

static void parserNext(Parser* parser) {
	parser->current = parser->lookahead;
	parser->lookahead = parser->input.p[parser->pos++ + 1];
}

static void parseWs(Parser* parser) {
loop:
	switch (parser->current) {
	case 0x20:
	case 0x09:
	case 0x0a:
	case 0x0d:
		parserNext(parser);
		goto loop;
	default:
		return;
	}
}

static Node parseValue(Parser* parser) {
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
	parserNext(parser);
	if (parser->current != 'a')
		goto error;
	parserNext(parser);
	if (parser->current != 'l')
		goto error;
	parserNext(parser);
	if (parser->current != 's')
		goto error;
	parserNext(parser);
	if (parser->current != 'e')
		goto error;
	parserNext(parser);
	return (Node){
		.type = N_BOOL,
		.start = start,
		.end = parser->pos,
		._bool = 0,
	};
ltrue:
	parserNext(parser);
	if (parser->current != 'r')
		goto error;
	parserNext(parser);
	if (parser->current != 'u')
		goto error;
	parserNext(parser);
	if (parser->current != 'e')
		goto error;
	parserNext(parser);
	return (Node){
		.type = N_BOOL,
		.start = start,
		.end = parser->pos,
		._bool = 1,
	};
lnull:
	parserNext(parser);
	if (parser->current != 'u')
		goto error;
	parserNext(parser);
	if (parser->current != 'l')
		goto error;
	parserNext(parser);
	if (parser->current != 'l')
		goto error;
	parserNext(parser);
	return (Node){
		.type = N_NULL,
		.start = start,
		.end = parser->pos,
	};
error:
	parserNext(parser);
	return (Node){
		.type = N_ERROR,
		.start = start,
		.end = parser->pos,
		.error = E_SYNTAX,
	};
}

Node parseJSON(Parser* parser) {
	parser->current = parser->input.p[parser->pos];
	parser->lookahead = parser->input.p[parser->pos + 1];
	parseWs(parser);
	return parseValue(parser);
}
