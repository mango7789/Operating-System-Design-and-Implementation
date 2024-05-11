#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>

bool valid_argument(char curr_arg, char *single_args[], char *double_args[]) {
	
	// single arguments
	for (int i = 0; *single_args[i] != '\0'; i++) {
		if (strcmp(&curr_arg, single_args[i]) == 0) {
			return true;
		}
	}

	// double arguments
	for (int i = 0; *double_args[i] != '\0'; i++) {
		if (strcmp(&curr_arg, double_args[i]) == 0) {
			return true;
		}
	}

	return false;
}

int main(int argc, char *argv[]) {

  	// set the optional arguments
	char *single_argument[] = {"-p", "-n", "-V"};
	char *double_argument[] = {"--show-pids", "--numeric-sort", "--version"};

	// iterate through the argv
	for (int i = 1; i < argc; i++) {
		assert(argv[i]);
		// printf("argv[%d] = %s\n", i, argv[i]);
		if (valid_argument(*argv[i], single_argument, double_argument) == false) {
			// fprintf(stderr, "Invalid argument! Please select from ['-p', '-n', '-V', '--show-pids', '--numeric-sort', '--version']");
			return 1;
		}
	}
	assert(!argv[argc]);
	return 0;
}
