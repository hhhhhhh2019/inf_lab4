#include "parser.h"
#include "json.h"
#include "utils.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define parse_value(file, field_name, parent_name)                             \
	do {                                                                       \
		if (parse_object(file, field_name, parent_name) != -1)                 \
			continue;                                                          \
		if (parse_array(file, field_name, parent_name) != -1)                  \
			continue;                                                          \
		if (parse_number(file, field_name, parent_name) != -1)                 \
			continue;                                                          \
		if (parse_string(file, field_name, parent_name) != -1)                 \
			continue;                                                          \
		if (parse_bool(file, field_name, parent_name) != -1)                   \
			continue;                                                          \
		if (parse_null(file, field_name, parent_name) != -1)                   \
			continue;                                                          \
	} while (0)

int parse_object(FILE* file, char* field_name, char* parent_name) {
	uint8_t type = read_uint8(file);

	if (type != N_OBJECT) {
		fseek(file, ftell(file) - 1, SEEK_SET);
		return -1;
	}

	char* current_name = NULL;

	if (field_name != NULL && parent_name == NULL) {
		size_t len = strlen(field_name);
		current_name = malloc(len + 1);
		strcpy(current_name, field_name);
	} else if (field_name != NULL && parent_name != NULL) {
		size_t len1 = strlen(parent_name);
		size_t len2 = strlen(field_name);
		current_name = malloc(len1 + 1 + len2 + 1);
		strcpy(current_name, parent_name);
		current_name[len1] = '.';
		strcpy(current_name + len1 + 1, field_name);
	}

	if (current_name != NULL) {
		printf("[%s]\n", current_name);
	}

	uint32_t count = read_uint32(file);

	for (int i = 0; i < count; i++) {
		char* name = read_string(file);
		parse_value(file, name, current_name);
		free(name);
	}

	if (current_name != NULL) {
		free(current_name);
	}

	return 0;
}

int parse_array(FILE* file, char* field_name, char* parent_name) {
	uint8_t type = read_uint8(file);

	if (type != N_ARRAY) {
		fseek(file, ftell(file) - 1, SEEK_SET);
		return -1;
	}

	size_t len = strlen(field_name);
	char* name = malloc(len + 2 + 1);
	strcpy(name, field_name);
	name[len + 0] = '[';
	name[len + 1] = ']';
	name[len + 2] = 0;

	uint32_t length = read_uint32(file);

	for (int i = 0; i < length; i++)
		parse_value(file, name, parent_name);

	return 0;
}

int parse_number(FILE* file, char* field_name, char* parent_name) {
	uint8_t type = read_uint8(file);

	if (type != N_NUMBER) {
		fseek(file, ftell(file) - 1, SEEK_SET);
		return -1;
	}

	printf("%s=%f\n", field_name, read_double(file));

	return 0;
}

int parse_string(FILE* file, char* field_name, char* parent_name) {
	uint8_t type = read_uint8(file);

	if (type != N_STRING) {
		fseek(file, ftell(file) - 1, SEEK_SET);
		return -1;
	}

	char* str = read_string(file);
	printf("%s=\"", field_name);
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
	printf("\"\n");
	free(str);

	return 0;
}

int parse_bool(FILE* file, char* field_name, char* parent_name) {
	uint8_t type = read_uint8(file);

	if (type != N_BOOL) {
		fseek(file, ftell(file) - 1, SEEK_SET);
		return -1;
	}

	printf("%s=\"%s\"\n", field_name, read_uint8(file) ? "true" : "false");

	return 0;
}

int parse_null(FILE* file, char* field_name, char* parent_name) {
	uint8_t type = read_uint8(file);

	if (type != N_NULL) {
		fseek(file, ftell(file) - 1, SEEK_SET);
		return -1;
	}

	printf("%s=\n", field_name);

	return 0;
}
