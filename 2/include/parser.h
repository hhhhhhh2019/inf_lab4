#ifndef PARSER_H_
#define PARSER_H_

#include <stdio.h>

int parse_object(FILE*, char* field_name, char* parent_name);
int parse_array(FILE*, char* field_name, char* parent_name);
int parse_number(FILE*, char* field_name, char* parent_name);
int parse_string(FILE*, char* field_name, char* parent_name);
int parse_bool(FILE*, char* field_name, char* parent_name);
int parse_null(FILE*, char* field_name, char* parent_name);

#endif // PARSER_H_
