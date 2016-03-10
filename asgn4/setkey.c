// Author: Adam Henry, adlhenry@ucsc.edu

#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>
#include <sys/setkey.h>

// Program name
char *execname;

// Print a program usage message to stderr
void usage ()
{
	fprintf(stderr,
	"Usage: %s [-e(--encrypt) | -d(--decrypt)] <key> <file>\n", execname);
	exit(EXIT_FAILURE);
}

// Parse program options
void parse_options (int argc, char **argv)
{
	int opt;
	static struct option longopts[] = {
	{"encrypt",	no_argument,	NULL,	'e'},
	{"decrypt",	no_argument,	NULL,	'd'},
	{NULL,				0,		NULL,	0}
	};

	if (argc != 4) {
		usage();
	}
	eflag = dflag = 0;
	while ((opt	= getopt_long(argc, argv, "ed", longopts, NULL)) != -1) {
		switch (opt) {
			case 'e':
				eflag = 1;
				break;
			case 'd':
				dflag = 1;
				break;
			default:
				usage();
		}
	}
}

int main (int argc, char **argv)
{
	execname = basename(argv[0]);
	parse_options(argc, argv);
}
