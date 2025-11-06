#ifndef UTILS_H_
#define UTILS_H_

#include <stdint.h>
#include <stdio.h>

void write_uint8(FILE*, uint8_t);
void write_uint32(FILE*, uint32_t);
void write_double(FILE*, double);
void write_string(FILE*, char*);

uint8_t read_uint8(FILE*);
uint32_t read_uint32(FILE*);
double read_double(FILE*);
char* read_string(FILE*);

#endif // UTILS_H_
