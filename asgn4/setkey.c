// Author: Adam Henry, adlhenry@ucsc.edu

#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>
#include <string.h>
//#include <sys/setkey.h>

// Program name
char *execname;

// Print a program usage message to stderr
void usage ()
{
	fprintf(stderr, "Usage: %s [-a | -r] <key1> <key2>\n", execname);
	exit(EXIT_FAILURE);
}

// Parse program options
void parse_options (int argc, char **argv)
{
	int opt;

	if (argc != 4) {
		usage();
	}
	while ((opt	= getopt(argc, argv, "ar")) != -1) {
		switch (opt) {
			case 'a':
				break;
			case 'r':
				break;
			default:
				usage();
		}
	}
}

int main (int argc, char **argv)
{
	int k0, k1;
	char key1[11];
	char key2[11];
	
	execname = basename(argv[0]);
	parse_options(argc, argv);
	
	char* key = argv[2];
	if (strlen(key) != 16) {
		fprintf(stderr, "ERROR: key %s is not 16 digits\n", key);
		exit(EXIT_FAILURE);
	}
	sprintf(key2, "0x%s", &key[8]);
	key[8] = '\0';
	sprintf(key1, "0x%s", key);
	
	k0 = strtol(key1, NULL, 0);
	k1 = strtol(key2, NULL, 0);
	
	//setkey(k0, k1);
	printf("%s: key <%08x%08x> set", execname, k0, k1);
	return EXIT_SUCCESS;
}
