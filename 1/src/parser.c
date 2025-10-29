#include "parser.h"
#include "json.h"

#ifdef DEBUG
#define LOG(...) fprintf(stderr, __VA_ARGS__)
#else
#define LOG(...)                                                               \
	{                                                                          \
	}
#endif

static void parser_next(Parser*);
static void parse_ws(Parser*);
static Node parse_value(Parser*);
// static Node parse_array(Parser*);
// static Node parse_object(Parser*);

static void parser_next(Parser* parser) {
	parser->current = parser->lookahead;
	if (parser->pos >= parser->input.len - 2)
		parser->lookahead = 0;
	else
		parser->lookahead = parser->input.p[++parser->pos + 1];

	LOG("%ld: %d %d\n", parser->pos, parser->current, parser->lookahead);
}

static void parse_ws(Parser* parser) {
	LOG("parse ws\n");
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
	LOG("parse value\n");
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

	LOG("%ld: %d %d\n", parser->pos, parser->current, parser->lookahead);

	parse_ws(parser);
	return parse_value(parser);
}

#ifdef DEBUG
void print_node(Node node, int offset) {
	for (int i = 0; i < offset; i++)
		LOG("  ");

	switch (node.type) {
	case N_ERROR:
		LOG("ERROR: %d\n", node.error);
		break;
	case N_BOOL:
		LOG(node._bool ? "true"
		               : "false"
		                 "\n");
		break;
	case N_NULL:
		LOG("NULL\n");
		break;
	case N_NUMBER:
		LOG("%f\n", node.number);
		break;
	case N_STRING:
		LOG("\"%s\"\n", node.string);
		break;
	case N_ARRAY:
		LOG("ARRAY: %zu\n", node.length);
		for (int i = 0; i < node.length; i++)
			print_node(node.elements[i], offset + 1);
		break;
	case N_OBJECT:
		LOG("OBJECT: %zu\n", node.count);
		for (int i = 0; i < node.count; i++) {
			LOG("%s:\n", node.fields[i].name);
			print_node(node.fields[i].value, offset + 1);
		}
		break;
	}
}
#endif
