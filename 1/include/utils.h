#ifndef UTILS_H_
#define UTILS_H_

#include <stdint.h>
#include <stdio.h>

void write_uint8(FILE*, uint8_t);
void write_uint32(FILE*, uint32_t);
void write_double(FILE*, double);
void write_string(FILE*, char*);

#endif // UTILS_H_
