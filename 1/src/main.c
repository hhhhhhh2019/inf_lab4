#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include "json.h"
#include "parser.h"

static void usage(char* name) {
	printf("Usage: %s <filename>\n", name);
}

int main(int argc, char* argv[]) {
	if (argc < 2) {
		usage(argv[0]);
		return 1;
	}

	FILE* finp = fopen(argv[1], "rb");

	if (finp == NULL) {
		perror("fopen");
		return errno;
	}

	Parser parser = parser_from_file(finp);

	fclose(finp);

	Node result = parse_JSON(&parser);
	print_node(result, 0);

	FILE* fout = stdout;

	if (argc > 2) {
		fout = fopen(argv[2], "wb");

		if (fout == NULL) {
			perror("fopen");
			return errno;
		}
	}

	node_to_bin(result, fout);

	if (argc > 2)
		fclose(fout);

	node_free(result);
	free(parser.input);
	free(parser.errors);
}
