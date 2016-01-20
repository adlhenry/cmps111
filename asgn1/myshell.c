// Author: Adam Henry, adlhenry@ucsc.edu

#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include <fcntl.h>
#include <libgen.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

extern char **getLine();

int exit_status = EXIT_SUCCESS;
char *execname = NULL;

// Print a warning to stderr
void errprintf (char *string) {
	fflush(NULL);
	fprintf(stderr, "%s: ", execname);
	if (string != NULL) fprintf(stderr, "%s\n", string);
	fflush(NULL);
	exit_status = EXIT_FAILURE;
}

// Print a system warning to stderr
void errnoprintf (char *command) {
	fflush(NULL);
	fprintf(stderr, "%s: ", command);
	fprintf(stderr, "%s\n", strerror(errno));
	fflush(NULL);
	exit_status = EXIT_FAILURE;
}

// Print the shell prompt
void print_prompt () {
	printf("$> ");
}

// Main loop
int main (int argc, char *argv[]) {
	execname = basename(argv[0]);
	if (argc > 1) {
		errprintf("usage: no options supported");
		return exit_status;
	}
	
	char *command = NULL;
	char **parameters = NULL;
	int status;
	
	while (true) {
		print_prompt();
		
		parameters = getLine();
		command = parameters[0];
		
		if (strcmp(command, "exit") == 0) {
			if (parameters[1] != NULL) {
				errprintf("exit: no options supported");
				continue;
			}
			exit(exit_status);
		}
		
		pid_t pid = fork();
		if (pid == -1) {
			errnoprintf(execname);
		} else if (pid != 0) {
			// Parent code
			int ret = waitpid(-1, &status, 0);
			if (ret == -1) errnoprintf(execname);
		} else {
			// Child code
			int ret = execvp(command, parameters);
			if (ret == -1) {
				errnoprintf(command);
				exit(exit_status);
			}
		}
		
	}
	
	return exit_status;
}
