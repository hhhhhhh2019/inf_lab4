#include <errno.h>
#include <json-c/json.h>
#include <json-c/json_object.h>
#include <json-c/json_types.h>
#include <stdio.h>

#include "json.h"
#include "utils.h"

void json2bin(json_object*, FILE*);

int main(int argc, char* argv[]) {
	if (argc == 1) {
		printf("Usage: %s <filename>\n", argv[0]);
		exit(1);
	}

	json_object* root = json_object_from_file(argv[1]);

	if (!root) {
		printf("failed to parse json\n");
		exit(1);
	}

	FILE* fout = stdout;

	if (argc >= 3) {
		fout = fopen(argv[2], "wb");

		if (!fout) {
			perror("fopen");
			exit(errno);
		}
	}

	json2bin(root, fout);

	json_object_put(root);
}

void json2bin(json_object* obj, FILE* file) {
	size_t len;
	switch (json_object_get_type(obj)) {
	case json_type_null:
		write_uint8(file, N_NULL);
		break;
	case json_type_boolean:
		write_uint8(file, N_BOOL);
		write_uint8(file, json_object_get_boolean(obj));
		break;
	case json_type_int:
		write_uint8(file, N_NUMBER);
		write_double(file, json_object_get_int(obj));
		break;
	case json_type_double:
		write_uint8(file, N_NUMBER);
		write_double(file, json_object_get_double(obj));
		break;
	case json_type_string:
		write_uint8(file, N_STRING);
		write_string(file, json_object_get_string(obj));
		break;
	case json_type_array:
		write_uint8(file, N_ARRAY);
		len = json_object_array_length(obj);
		write_uint32(file, len);
		for (int i = 0; i < len; i++)
			json2bin(json_object_array_get_idx(obj, i), file);
		break;
	case json_type_object:
		write_uint8(file, N_OBJECT);
		len = json_object_object_length(obj);
		write_uint32(file, len);
		json_object_object_foreach(obj, key, val) {
			write_string(file, key);
			json2bin(val, file);
		}
		break;
	}
}
