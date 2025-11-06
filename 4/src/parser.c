#include "parser.h"
#include "json.h"
#include "utils.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define INDENT(indent)                                                         \
	do {                                                                       \
		for (int i = 0; i < (indent); i++)                                     \
			printf("  ");                                                      \
	} while (0)

int parse_value(FILE* file, int indent) {
	if (parse_object(file, indent) != -1)
		return 0;
	if (parse_array(file, indent) != -1)
		return 0;
	if (parse_number(file, indent) != -1)
		return 0;
	if (parse_string(file, indent) != -1)
		return 0;
	if (parse_bool(file, indent) != -1)
		return 0;
	if (parse_null(file, indent) != -1)
		return 0;
	return -1;
}

int parse_object(FILE* file, int indent) {
	uint8_t type = read_uint8(file);

	if (type != N_OBJECT) {
		fseek(file, ftell(file) - 1, SEEK_SET);
		return -1;
	}

	INDENT(indent);

	printf("<object>\n");
	uint32_t count = read_uint32(file);
	for (int i = 0; i < count; i++) {
		INDENT(indent + 1);
		printf("<member name=\"");
		char* name = read_string(file);
		for (int j = 0; j < strlen(name); j++) {
			switch (name[j]) {
			case '"':
				printf("\\\"");
				break;
			case '\n':
				printf("\\n");
				break;
			case '\b':
				printf("\\b");
				break;
			case '\t':
				printf("\\t");
				break;
			case '\r':
				printf("\\r");
				break;
			case '\f':
				printf("\\f");
				break;
			case '\\':
				printf("\\\\");
				break;
			default:
				printf("%c", name[j]);
			}
		}
		printf("\">\n");
		parse_value(file, indent + 2);
		INDENT(indent + 1);
		printf("<member/>\n");
	}
	printf("<object/>\n");

	return 0;
}

int parse_array(FILE* file, int indent) {
	uint8_t type = read_uint8(file);

	if (type != N_ARRAY) {
		fseek(file, ftell(file) - 1, SEEK_SET);
		return -1;
	}

	INDENT(indent);

	printf("<array>\n");
	uint32_t length = read_uint32(file);
	for (int i = 0; i < length; i++)
		parse_value(file, indent + 1);
	printf("<array/>\n");

	return 0;
}

int parse_number(FILE* file, int indent) {
	uint8_t type = read_uint8(file);

	if (type != N_NUMBER) {
		fseek(file, ftell(file) - 1, SEEK_SET);
		return -1;
	}

	INDENT(indent);

	printf("<number value=%f/>\n", read_double(file));

	return 0;
}

int parse_string(FILE* file, int indent) {
	uint8_t type = read_uint8(file);

	if (type != N_STRING) {
		fseek(file, ftell(file) - 1, SEEK_SET);
		return -1;
	}

	INDENT(indent);

	printf("<string value=\"");
	char* str = read_string(file);
	for (int i = 0; i < strlen(str); i++) {
		switch (str[i]) {
		case '"':
			printf("\\\"");
			break;
		case '\n':
			printf("\\n");
			break;
		case '\b':
			printf("\\b");
			break;
		case '\t':
			printf("\\t");
			break;
		case '\r':
			printf("\\r");
			break;
		case '\f':
			printf("\\f");
			break;
		case '\\':
			printf("\\\\");
			break;
		default:
			printf("%c", str[i]);
		}
	}
	printf("\"/>\n");

	return 0;
}

int parse_bool(FILE* file, int indent) {
	uint8_t type = read_uint8(file);

	if (type != N_BOOL) {
		fseek(file, ftell(file) - 1, SEEK_SET);
		return -1;
	}

	INDENT(indent);

	printf("<bool value=%s/>\n", read_uint8(file) ? "true" : "false");

	return 0;
}

int parse_null(FILE* file, int indent) {
	uint8_t type = read_uint8(file);

	if (type != N_NULL) {
		fseek(file, ftell(file) - 1, SEEK_SET);
		return -1;
	}

	INDENT(indent);

	printf("<null/>\n");

	return 0;
}
