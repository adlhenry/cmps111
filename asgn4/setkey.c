// Author: Adam Henry, adlhenry@ucsc.edu

#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>
#include <unistd.h>
#include <string.h>
#include <sys/syscall.h>

// Program name
char *execname;

// Add and remove global flags
int aflag, rflag;

// Key global
char *key;

// Print a program usage message to stderr
void usage ()
{
	fprintf(stderr, "Usage: %s [-a <key> | -r]\n", execname);
	exit(EXIT_FAILURE);
}

// Parse program options
void parse_options (int argc, char **argv)
{
	int opt;

	if (argc < 2 || argc > 3) {
		usage();
	}
	aflag = rflag = 0;
	while ((opt	= getopt(argc, argv, "a:r")) != -1) {
		switch (opt) {
			case 'a':
				aflag = 1;
				key = optarg;
				break;
			case 'r':
				rflag = 1;
				break;
			default:
				usage();
		}
	}
}

// Initialize arguments for setkey() from a 16-digit hex string
void set_keys (int *k0, int *k1)
{
	char key1[11];
	char key2[11];
	
	if (strlen(key) != 16) {
		fprintf(stderr, "%s: key <%s> is not 16 digits\n", execname, key);
		exit(EXIT_FAILURE);
	}
	snprintf(key1, 11, "0x%s", key);
	snprintf(key2, 11, "0x%s", &key[8]);
	*k0 = strtol(key1, NULL, 0);
	*k1 = strtol(key2, NULL, 0);
	if (k0 == 0 && k1 == 0) {
		fprintf(stderr, "%s: key <%s> is invalid\n", execname, key);
		exit(EXIT_FAILURE);
	}
}

// Call setkey() and check for failure
void set_kernkey (int k0, int k1)
{
	if (syscall(546, k0, k1) != 0) {
		fprintf(stderr, "%s: unable to modify the key\n", execname);
		exit(EXIT_FAILURE);
	}
}

int main (int argc, char **argv)
{
	int k0, k1;
	
	execname = basename(argv[0]);
	parse_options(argc, argv);
	if (aflag) {
		set_keys(&k0, &k1);
		set_kernkey(k0, k1);
		printf("%s: key <%08x%08x> set\n", execname, k0, k1);
	}
	if (rflag) {
		k0 = k1 = 0;
		set_kernkey(k0, k1);
		printf("%s: key removed\n", execname);
	}
	return EXIT_SUCCESS;
}
