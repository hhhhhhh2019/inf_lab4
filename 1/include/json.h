#ifndef JSON_H_
#define JSON_H_


#include <stddef.h>


enum ErrorType {
	E_SYNTAX,
};

enum NodeType {
	N_NULL,
	N_BOOL,
	N_NUMBER,
	N_STRING,
	N_ARRAY,
	N_OBJECT,
	N_ERROR,
};

typedef struct Node Node;
typedef struct ObjectField ObjectField;

struct Node {
	enum NodeType type;

	unsigned int start;
	unsigned int end;

	union {
		enum ErrorType error;
		double number;
		char _bool;
		char* string;
		struct {
			size_t length;
			Node* elements;
		};
		struct {
			size_t count;
			ObjectField* fields;
		};
	};
};

struct ObjectField {
	char* name;
	Node value;
};


#endif
