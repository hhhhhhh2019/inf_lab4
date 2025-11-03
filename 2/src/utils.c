#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
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

uint8_t read_uint8(FILE* file) {
	uint8_t result;
	fread(&result, 1, 1, file);
	return result;
}

uint32_t read_uint32(FILE* file) {
	uint32_t result;
	unsigned char buf[4];
	fread(&buf, 4, 1, file);
	for (int i = 0; i < 4; i++)
		result |= buf[i] << i * 8;
	return result;
}

double read_double(FILE* file) {
	uint64_t result;
	unsigned char buf[8];
	fread(&buf, 8, 1, file);
	for (int i = 0; i < 8; i++)
		result |= buf[i] << i * 8;
	return *((double*)&result);
}

char* read_string(FILE* file) {
	uint32_t len = read_uint32(file);
	char* s = malloc(len + 1);
	fread(s, len, 1, file);
	s[len] = 0;
	return s;
}
