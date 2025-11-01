#include "utils.h"

#include <stdio.h>
#include <string.h>

void write_uint8(FILE* file, uint8_t num) {
	fwrite(&num, 1, 1, file);
}

void write_uint32(FILE* file, uint32_t num) {
	fwrite(&num, 4, 1, file);
}

void write_double(FILE* file, double num) {
	fwrite(&num, sizeof(num), 1, file);
}

void write_string(FILE* file, char* str) {
	write_uint32(file, strlen(str));
	fwrite(str, strlen(str), 1, file);
}
