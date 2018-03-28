#include "dmp.h"





void
printcharbuff(int count, char* buff) {
	int i = 0;
	printf(" |  ");
	for (i = 0; i < count; i++) {
		char c = buff[i];
		if (c == '.') printf("\x1b[2m");
		printf("%c"ANSI_COLOR_RESET, c);
	}
	printf("\n");
}


int
main(int argc, char** argv) {


	int linesize = 0xf;

	static struct option const long_options[] = {
		{"linecount", required_argument, 0, 'l'},
		{NULL, 0, NULL, 0} 
	};


	char o = 0;
	while ((o = getopt_long(argc, argv, "l:", long_options, NULL)) != -1) {
		switch (o) {
			case 'l':
				linesize = atoi(optarg);
				break;
		}
	}


	if (optind >= argc) {
		input = stdin;
	} else {
		input = fopen(argv[optind], "r");
	}


	int i = 0;
	char c = 0;

	char* buff = calloc(1, linesize);

	while (!feof(input)) {
		c = fgetc(input) & 0xff;
		// Multiple of 16 means new line (with line offset).
		if ((i % linesize) == 0) {
			// Just don't print ASCII for the zeroth line.
			if (i != 0)
				printcharbuff(linesize, buff);
			// Output the offset.
			printf("%07x |", i);
		}
		// Now the hex code for the specific character.
		if (!(c > 0x20 && c < 0x7e)) printf("\x1b[2m");
		printf(" %02x" ANSI_COLOR_RESET, (unsigned char)c);
		// And store a printable ASCII character for later.
		if ((c < 0x20) || (c > 0x7e)) {
			buff[i % linesize] = '.';
		} else {
			buff[i % linesize] = c;
		}
		buff[(i % linesize) + 1] = '\0';
		i++;
	}

	// Pad out last line if not exactly count characters.
	while ((i % linesize) != 0) {
		printf("   ");
		i++;
	}
	printcharbuff(linesize, buff);



	
	return 0;
}
