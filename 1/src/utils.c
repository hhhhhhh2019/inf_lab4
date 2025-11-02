#include "utils.h"

#include <stdio.h>
#include <string.h>

void write_uint8(FILE* file, uint8_t num) {
	fwrite(&num, 1, 1, file);
}

void write_uint32(FILE* file, uint32_t num) {
	unsigned char buf[4];
	for (int i = 0; i < 4; i++)
		buf[i] = (num >> i * 8) & 0xff;
	fwrite(buf, 4, 1, file);
}

void write_double(FILE* file, double num) {
	unsigned char buf[8];
	for (int i = 0; i < 8; i++)
		buf[i] = (*((uint64_t*)&num) >> i * 8) & 0xff;
	fwrite(buf, 8, 1, file);
}

void write_string(FILE* file, char* str) {
	write_uint32(file, strlen(str));
	fwrite(str, strlen(str), 1, file);
}
