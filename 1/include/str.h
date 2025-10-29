#ifndef STRING_H_
#define STRING_H_

#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

typedef struct {
	const char* p;
	size_t len;
} Str;

#define STRING(str)                                                            \
	(Str) {                                                                    \
		.p = str, .len = strlen(str)                                           \
	}

static inline Str str_from(const char* str) {
	return (Str){.p = str, .len = strlen(str)};
}

static inline Str str_slice(Str str, size_t start, size_t end) {
	assert(end >= start);
	return (Str){
		.p = str.p + start,
		.len = end - start,
	};
}

Str read_to_str(FILE*);

#endif // STRING_H_
