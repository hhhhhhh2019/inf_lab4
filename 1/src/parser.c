#include "parser.h"

// static void parserNext(Parser*);
static Node parseValue(Parser*);
// static Node parseArray(Parser*);
// static Node parseObject(Parser*);

// static void parserNext(Parser* parser) {
// 	parser->current = parser->lookahead;
// 	parser->lookahead = parser->input.p[parser->pos++ + 1];
// }

static Node parseValue(Parser* parser) {
	return (Node){
	};
}

Node parseJSON(Parser* parser) {
	parser->current = parser->input.p[parser->pos];
	parser->lookahead = parser->input.p[parser->pos + 1];
	return parseValue(parser);
}
