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
	fprintf(stderr, "%s\n", string);
	fflush(NULL);
	exit_status = EXIT_FAILURE;
}

// Print the shell prompt
void print_prompt () {
	printf("$> ");
}

char *outfile = NULL;
char *infile = NULL;
char **argvs[10];
char ***getLine2 () {
	char **args = getLine();
	argvs[0] = args;
	int j = 0;
	for (int i = 0; args[i] != NULL; i++) {
		switch (args[i][0]) {
			case '>':
				args[i] = NULL;
				outfile = args[++i];
				break;
			case '<':
				args[i] = NULL;
				infile = args[++i];
				break;
			case '|':
				args[i] = NULL;
				argvs[j] = &args[i];
				break;
		}
	}
	return argvs;
}

/*
int stdout_copy;
int output_redirect (char *filename) {
	if (filename == NULL) return -1;
	stdout_copy = dup(STDOUT_FILENO);
	close(STDOUT_FILENO);
	int fd = open(filename, O_WRONLY|O_CREAT|O_TRUNC, 00644);
	if (fd == -1) perror(filename);
	return fd;
}

void output_redirect_close (int fd) {
	if (fd == -1) return;
	close(fd);
	dup(stdout_copy);
}
*/

void redirect_out (char *filename) {
	if (filename == NULL) return;
	close(STDOUT_FILENO);
	int fd = creat(filename, 00644);
	if (fd == -1) perror(filename);
}

void redirect_in (char *filename) {
	if (filename == NULL) return;
	close(STDIN_FILENO);
	int fd = open(filename, O_RDONLY);
	if (fd == -1) perror(filename);
}

bool is_exit (char **args) {
	if (strcmp(args[0], "exit") == 0) {
		if (args[1] != NULL) {
			errprintf("exit: no options supported");
			return true;
		}
		exit(exit_status);
	}
	return false;
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
		
		outfile = NULL;
		infile = NULL;
		char ***argvs = getLine2();
		parameters = argvs[0];
		command = parameters[0];
		
		if (is_exit(parameters)) continue;
		
		pid_t pid = fork();
		if (pid == -1) {
			perror(execname);
		} else if (pid != 0) {
			// Parent code
			int ret = waitpid(-1, &status, 0);
			if (ret == -1) perror(execname);
		} else {
			// Child code
			redirect_out(outfile);
			redirect_in(infile);
			int ret = execvp(command, parameters);
			if (ret == -1) {
				perror(command);
				exit(exit_status);
			}
		}

	}
	
	return exit_status;
}
