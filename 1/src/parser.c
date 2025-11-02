#include "parser.h"
#include "json.h"
#include "utils.h"

#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

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
static Node parse_number(Parser*);
static Node parse_string(Parser*);
static Node parse_array(Parser*);
static Node parse_object(Parser*);

static void parser_next(Parser* parser) {
	parser->current = parser->lookahead;
	if (parser->pos >= parser->input_len - 2)
		parser->lookahead = 0;
	else
		parser->lookahead = parser->input[++parser->pos + 1];

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

static Node parse_number(Parser* parser) {
	LOG("parse number\n");
	unsigned int start = parser->pos;

	double sign = 1;
	double _int = 0;
	double frac = 0;
	double exp = 0;
	double exp_sign = 1;

	if (parser->current == '-') {
		sign = -1;
		parser_next(parser);
	}

int_part:
	switch (parser->current) {
	case '0':
		parser_next(parser);
		goto frac_part;
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
		_int = _int * 10 + parser->current - '0';
		parser_next(parser);
		break;
	default:
		goto error;
	}

int_part_loop:
	switch (parser->current) {
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
		_int = _int * 10 + parser->current - '0';
		parser_next(parser);
		goto int_part_loop;
	default:
		break;
	}

frac_part:
	if (parser->current != '.')
		goto exp_part;

	double frac_mul = 0.1;
	parser_next(parser);

frac_part_loop:
	// TODO: first char is non digit error check
	switch (parser->current) {
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
		frac += (parser->current - '0') * frac_mul;
		frac_mul *= 0.1;
		parser_next(parser);
		goto frac_part_loop;
	default:
		break;
	}

exp_part:
	if (parser->current != 'e' && parser->current != 'E')
		goto result;

	parser_next(parser);

	switch (parser->current) {
	case '-':
		exp_sign = -1;
	case '+':
		parser_next(parser);
	default:
		break;
	}

exp_part_loop:
	// TODO: first non digit check
	switch (parser->current) {
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
		exp = exp * 10 + parser->current - '0';
		parser_next(parser);
		goto exp_part_loop;
	default:
		break;
	}

result:
	LOG("number: %f %f %f %f %f\n", sign, _int, frac, exp_sign, exp);
	return (Node){
		.type = N_NUMBER,
		.number = (_int + frac) * pow(10, exp * exp_sign) * sign,
	};

error:
	// TODO
	LOG("number failed\n");
	parser_add_error(parser, (Error){
								 .type = E_SYNTAX,
								 .start = start,
								 .end = parser->pos,
							 });
	return (Node){
		.type = N_NULL,
	};
}

static Node parse_string(Parser* parser) {
	LOG("parse string\n");
	unsigned int start = parser->pos;
	char* string = malloc(1);
	*string = 0;

	parser_next(parser); // skip "

	while (1) {
		if (parser->current == 0) {
			// TODO
			LOG("string failed\n");
			parser_add_error(parser, (Error){
										 .type = E_SYNTAX,
										 .start = start,
										 .end = parser->pos,
									 });
			return (Node){
				.type = N_NULL,
			};
		}

		if (parser->current == '"')
			break;

		else if ((parser->current >> 5) == 0b110) {
			int len = strlen(string);
			string = realloc(string, len + 3);

			string[len] = parser->current;
			parser_next(parser);
			string[len + 1] = parser->current;
			parser_next(parser);
			string[len + 2] = 0;

			continue;
		}

		else if ((parser->current >> 4) == 0b1110) {
			int len = strlen(string);
			string = realloc(string, len + 4);

			string[len] = parser->current;
			parser_next(parser);
			string[len + 1] = parser->current;
			parser_next(parser);
			string[len + 2] = parser->current;
			parser_next(parser);
			string[len + 3] = 0;

			continue;
		}

		else if ((parser->current >> 3) == 0b11110) {
			int len = strlen(string);
			string = realloc(string, len + 5);

			string[len] = parser->current;
			parser_next(parser);
			string[len + 1] = parser->current;
			parser_next(parser);
			string[len + 2] = parser->current;
			parser_next(parser);
			string[len + 3] = parser->current;
			parser_next(parser);
			string[len + 4] = 0;

			continue;
		} else {
			int len = strlen(string);
			string = realloc(string, len + 2);
			string[len + 1] = 0;

			if (parser->current != '\\') {
				string[len] = parser->current;
				parser_next(parser);
				continue;
			}

			parser_next(parser);

			switch (parser->current) {
			case '"':
				string[len] = 0x22;
				parser_next(parser);
				break;
			case '\\':
				string[len] = 0x5c;
				parser_next(parser);
				break;
			case '/':
				string[len] = 0x2f;
				parser_next(parser);
				break;
			case 'b':
				string[len] = 0x8;
				parser_next(parser);
				break;
			case 'f':
				string[len] = 0xc;
				parser_next(parser);
				break;
			case 'n':
				string[len] = 0xa;
				parser_next(parser);
				break;
			case 'r':
				string[len] = 0xd;
				parser_next(parser);
				break;
			case 't':
				string[len] = 0x9;
				parser_next(parser);
				break;
			case 'u':
				// TODO
				break;
			default:
				// TODO
				LOG("string undefined escape sequence\n");
				parser_add_error(parser, (Error){
											 .type = E_SYNTAX,
											 .start = parser->pos - 1,
											 .end = parser->pos,
										 });
				parser_next(parser);
				break;
			}
		}
	}

	parser_next(parser);

	return (Node){
		.type = N_STRING,
		.string = string,
	};
}

static Node parse_array(Parser* parser) {
	LOG("parse array\n");
	unsigned int start = parser->pos;
	parser_next(parser);

	Node* elements = malloc(0);
	size_t length = 0;

	if (parser->current == ']')
		goto result;

	while (1) {
		parse_ws(parser);

		elements = realloc(elements, sizeof(Node) * (++length));
		elements[length - 1] = parse_value(parser);
		parse_ws(parser);

		if (parser->current == ']')
			break;

		if (parser->current == ',') {
			parser_next(parser);
			continue;
		}

		// TODO
		parser_add_error(parser, (Error){
									 .type = E_SYNTAX,
									 .start = start,
									 .end = parser->pos,
								 });
	}

result:
	parser_next(parser);
	return (Node){
		.type = N_ARRAY,
		.elements = elements,
		.length = length,
	};
}

static Node parse_object(Parser* parser) {
	LOG("parse object\n");
	unsigned int start = parser->pos;
	parser_next(parser);

	ObjectField* fields = malloc(0);
	size_t count = 0;

	if (parser->current == '}')
		goto result;

	while (1) {
		parse_ws(parser);

		if (parser->current != '"') {
			// TODO
			parser_add_error(parser, (Error){
										 .type = E_SYNTAX,
										 .start = start,
										 .end = parser->pos,
									 });
		}

		Node name = parse_string(parser);

		if (name.type == N_NULL) {
			// TODO
			assert(0);
		}

		parse_ws(parser);

		if (parser->current != ':') {
			// TODO
			parser_add_error(parser, (Error){
										 .type = E_SYNTAX,
										 .start = start,
										 .end = parser->pos,
									 });
			assert(0);
		}

		parser_next(parser);

		parse_ws(parser);

		fields = realloc(fields, sizeof(ObjectField) * (++count));
		fields[count - 1].name = name.string;
		fields[count - 1].value = parse_value(parser);

		parse_ws(parser);

		if (parser->current == '}')
			break;

		if (parser->current == ',') {
			parser_next(parser);
			continue;
		}

		// TODO
		parser_add_error(parser, (Error){
									 .type = E_SYNTAX,
									 .start = start,
									 .end = parser->pos,
								 });
	}

result:
	parser_next(parser);
	return (Node){
		.type = N_OBJECT,
		.fields = fields,
		.count = count,
	};
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
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	case '-':
		return parse_number(parser);
	case '"':
		return parse_string(parser);
	case '[':
		return parse_array(parser);
	case '{':
		return parse_object(parser);
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
	};
error:
	parser_next(parser);
	parser_add_error(parser, (Error){
								 .type = E_SYNTAX,
								 .start = start,
								 .end = parser->pos,
							 });
	return (Node){
		.type = N_NULL,
	};
}

Node parse_JSON(Parser* parser) {
	parser->current = parser->input[parser->pos];
	parser->lookahead = parser->input[parser->pos + 1];

	LOG("%ld: %d %d\n", parser->pos, parser->current, parser->lookahead);

	parse_ws(parser);
	Node result = parse_value(parser);
	parse_ws(parser);
	return result;
}

void node_free(Node node) {
	switch (node.type) {
	case N_ARRAY:
		for (int i = 0; i < node.length; i++)
			node_free(node.elements[i]);
		free(node.elements);
		break;
	case N_OBJECT:
		for (int i = 0; i < node.count; i++) {
			node_free(node.fields[i].value);
			free(node.fields[i].name);
		}
		free(node.fields);
		break;
	case N_STRING:
		free(node.string);
		break;
	default:
		break;
	}
}

void node_to_bin(Node node, FILE* file) {
	write_uint8(file, node.type);
	switch (node.type) {
	case (N_NULL):
		break;
	case (N_BOOL):
		write_uint8(file, node._bool);
		break;
	case (N_NUMBER):
		write_double(file, node.number);
		break;
	case (N_STRING):
		write_string(file, node.string);
		break;
	case (N_ARRAY):
		write_uint32(file, node.length);
		for (int i = 0; i < node.length; i++)
			node_to_bin(node.elements[i], file);
		break;
	case (N_OBJECT):
		write_uint32(file, node.count);
		for (int i = 0; i < node.count; i++) {
			write_string(file, node.fields[i].name);
			node_to_bin(node.fields[i].value, file);
		}
		break;
	}
}

#ifdef DEBUG
void print_node(Node node, int offset) {
	for (int i = 0; i < offset; i++)
		LOG("  ");

	switch (node.type) {
	case N_BOOL:
		LOG(node._bool ? "true\n" : "false\n");
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
			for (int i = 0; i < offset + 1; i++)
				LOG("  ");
			LOG("%s\n", node.fields[i].name);
			print_node(node.fields[i].value, offset + 1);
		}
		break;
	}
}
#endif
