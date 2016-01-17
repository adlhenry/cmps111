// Author: Adam Henry, adlhenry@ucsc.edu

#include <stdlib.h>
#include <stdbool.h>
#include <fcntl.h>
#include <libgen.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>

extern char **getLine();

int exit_status = EXIT_SUCCESS;
char *execname = NULL;

void errprintf (char *string) {
	fflush(NULL);
	fprintf(stderr, "%s: ", execname);
	if (string != NULL) fprintf(stderr, "%s\n", string);
	fflush(NULL);
	exit_status = EXIT_FAILURE;
}

int main (int argc, char *argv[]) {
	execname = basename(argv[0]);
	
	int i;
	char **args;
	
	while (true) {
		args = getLine();
		for (i = 0; args[i] != NULL; i++) {
			printf("Argument %d: %s\n", i, args[i]);
		}
	}
	
	return exit_status;
}
