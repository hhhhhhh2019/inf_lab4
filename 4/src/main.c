#include "json.h"
#include "parser.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

void usage(char* name) {
	printf("Usage: %s <filename>\n", name);
}

int main(int argc, char* argv[]) {
	if (argc < 2) {
		usage(argv[0]);
		exit(1);
	}

	FILE* finp = fopen(argv[1], "rb");
	FILE* fout = stdout;

	if (finp == NULL) {
		perror("fopen");
		exit(errno);
	}

	if (argc > 2) {
		fout = fopen(argv[2], "w");
		if (fout == NULL) {
			perror("fopen");
			exit(errno);
		}
	}

	parse_value(finp, 0);

	fclose(fout);
	fclose(finp);
}
