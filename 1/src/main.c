#include <stdio.h>
#include <errno.h>

#include "json.h"


static void usage(char* name) {
	printf("Usage: %s <filename>\n", name);
}


int main(int argc, char* argv[]) {
	if (argc != 2) {
		usage(argv[0]);
		return 1;
	}

	FILE* finp = fopen(argv[1], "rb");

	if (finp == NULL) {
		perror("fopen");
		return errno;
	}

	fclose(finp);
}
