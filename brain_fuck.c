#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_SIZE 256
typedef unsigned char bfcell;

int main(int argc, char *argv[]) {

	FILE *fin = (argc > 1) ? fopen(argv[1], "r") : stdin;

	if (fin == NULL) {
		fprintf(stderr, "Unable to open target file named: %s\n", argv[1]);
		return EXIT_FAILURE;
	}

	bfcell *ptr = calloc(30000, sizeof (bfcell));
	bfcell *bf_tape_start = ptr;
	if (ptr == NULL) {
		fprintf(stderr, "Memory allocation for Brain Fck tape failed\n");
		return EXIT_FAILURE;
	}

	char ln_buffer[MAX_LINE_SIZE];

	int avlbmem = 4096;
	char *main_buffer = malloc(avlbmem);
	if (main_buffer == NULL) {
		fprintf(stderr, "Memory allocation for main_buffer failed\n");
		return EXIT_FAILURE;
	}

	char *strpos = main_buffer;
	int buffer_len;
	int memusg = 0;

	while (fgets(ln_buffer, MAX_LINE_SIZE, fin) != NULL) {
		buffer_len = strlen(ln_buffer);
		memusg += buffer_len;

		if (memusg >= avlbmem) {
			avlbmem *= 2;
			main_buffer = realloc(main_buffer, avlbmem);
			if (main_buffer == NULL) {
				fprintf(stderr, "Unable to reallocate memory for main_buffer");
				return EXIT_FAILURE;
			}
		}
		memcpy(strpos, ln_buffer, buffer_len);
		strpos += buffer_len;
	}

	int ln_no = 1;
	int lbal_st = 0;
	int lbal_end = 0;

	char *buf_start = main_buffer;
	do {

		switch (*main_buffer) {

		case '\n':
			ln_no++;
			break;

		case '+':
			(*ptr)++;
			break;

		case '-':
			(*ptr)--;
			break;

		case '>':
			ptr++;
			break;

		case '<':
			ptr--;
			break;

		case '.':
			putchar(*ptr);
			break;

		case ',':
			(*ptr) = getchar();
			break;

		case '[':
			if (*ptr == 0) {
				lbal_st++;
				while (lbal_st) {
					switch (*(++main_buffer)) {

					case '[':
						lbal_st++;
						break;

					case ']':
						lbal_st--;
						break;
					}
				}
			}
			break;

		case ']':
			lbal_end++;
			while (lbal_end) {
				switch(*(--main_buffer)) {

				case '[':
					lbal_end--;
					break;

				case ']':
					lbal_end++;
					break;
				}
			}
			main_buffer--;
			break;
		}

	} while(++main_buffer < strpos);

	free(bf_tape_start);
	free(buf_start);

	return EXIT_SUCCESS;
}


