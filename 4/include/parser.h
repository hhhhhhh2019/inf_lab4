#ifndef PARSER_H_
#define PARSER_H_

#include <stdio.h>

int parse_value(FILE*, int indent);
int parse_object(FILE*, int indent);
int parse_array(FILE*, int indent);
int parse_number(FILE*, int indent);
int parse_string(FILE*, int indent);
int parse_bool(FILE*, int indent);
int parse_null(FILE*, int indent);

#endif // PARSER_H_
