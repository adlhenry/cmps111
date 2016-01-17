// Author: Adam Henry, adlhenry@ucsc.edu

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <libgen.h>

int exit_status = EXIT_SUCCESS;
char *execname = NULL;

void errprintf (char *string) {
	fflush(NULL);
	fprintf(stderr, "%s: ", execname);
	if (string != NULL) fprintf(stderr, "%s\n", string);
	fflush(NULL);
	exit_status = EXIT_FAILURE;
}

void cat (char *filename) {
	size_t buffer_len = 4096, r_bytes, w_bytes;
	char buffer[buffer_len];
	
	int file = open(filename, O_RDONLY);
	if (file == -1) {
		errprintf(NULL);
		perror(filename);
		return;
	}
	
	while ((r_bytes = read(file, &buffer, buffer_len)) > 0) {
		w_bytes = write(STDOUT_FILENO, &buffer, r_bytes);
		if (w_bytes != r_bytes) {
			errprintf(NULL);
			perror("");
			return;
		}
	}
	close(file);
}

int main (int argc, char *argv[]) {
	execname = basename(argv[0]);
	if (argc < 2) {
		errprintf ("usage: cat filename [filename ...]");
		return exit_status;
	}
	for (int i = 1; i < argc; i++) {
		cat(argv[i]);
	}
	return exit_status;
}
