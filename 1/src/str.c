#include "str.h"

#include <stdio.h>
#include <stdlib.h>

Str read_to_str(FILE* f) {
	size_t offset = ftell(f);

	fseek(f, 0, SEEK_END);
	size_t len = ftell(f) - offset;
	fseek(f, offset, SEEK_SET);

	char* buf = malloc(len);
	fread(buf, len, 1, f);

	return (Str){
		.p = buf,
		.len = len,
	};
}
