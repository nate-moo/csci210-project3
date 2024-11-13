#include <stdio.h>
#include <stdlib.h>
#include <spawn.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

#define N 12

extern char **environ;

char *allowed[N] = {"cp","touch","mkdir","ls","pwd","cat","grep","chmod","diff","cd","exit","help"};

int isAllowed(const char*cmd, char** cmdline, int* offset) {
	// TODO
	// return 1 if cmd is one of the allowed commands
	// return 0 otherwise

	char* copy = memcpy(cmd, &cmd[0], 256);

	int done = 0;

	if (cmd[strlen(cmd) - 1] != ' ') {
		for (int i = 0; i < strlen(cmd); i++) {
			if (cmd[i] == ' ') {
				done = 1;
				memcpy(*cmdline, &cmd[0], i);
				*offset = i + 1;
				break;
			}
		}
	} else {
		copy[strlen(cmd) - 1] = '\0';
	}

	if (done != 1) {
		memcpy(*cmdline, &copy[0], strlen(cmd));
		*offset = -1;
	}

	// printf("%s", *cmdline);

	for (int i = 0; i < N; i++) {
		if (!strcmp(allowed[i], *cmdline)) {
			return 1;
		}
	}
	
	return 0;
}

int spawn(char *argv[]) {

	// printf("argv: ..%s.. - ..%s.. - ..%s..", argv[0], argv[1], argv[2]);

	pid_t pid;
	// char *argv[] = {"echo", "Hello from the spawned process!", NULL};
	int status;
	posix_spawnattr_t attr;

	// Initialize spawn attributes
	posix_spawnattr_init(&attr);

	// Spawn a new process
	if (posix_spawnp(&pid, "echo", NULL, &attr, argv, environ) != 0) {
		perror("spawn failed");
		return -1;
	}

	printf("PID: %d", pid);

	// Wait for the spawned process to terminate
	if (waitpid(pid, &status, 0) == -1) {
		perror("waitpid failed");
		return -1;
	}

	return 0;
}

int main() {

    // TODO
    // Add variables as needed

    char line[256];

    while (1) {

		fprintf(stderr,"rsh>");

		if (fgets(line,256,stdin)==NULL) continue;

		if (strcmp(line,"\n")==0) {
			// printf("newline", line);
			continue;
		}

		line[strlen(line)-1]='\0';

    	int offset = 0;
    	char* cmd = malloc(sizeof(char) * 256);
    	char* argv[4] = {
    		malloc(sizeof(char) * 256/4),
    		malloc(sizeof(char) * 256/4),
    		malloc(sizeof(char) * 256/4),
    		malloc(sizeof(char) * 256/4),
    	};

    	if (isAllowed(line, &cmd, &offset) == 0) {
    		printf("NOT ALLOWED!\n");
    		continue;
    	}

    	argv[0] = cmd;
    	int argc = 1;

    	if (offset == -1) {
    		printf("");
    	} else {

    		int j = 0;
    		// offset++;
    		int previousOffset = offset;

		    for (int i = offset; i <= strlen(line); ++i) {
			    if (line[i] == ' ' || line[i] == '\0') {
			    	memcpy(argv[argc], &line[previousOffset], j);
			    	argv[argc][j] = '\0';
			    	previousOffset = previousOffset + j + 1;
			    	argc++;
			    }
		    	j++;
		    }

    		// argv[argc] = NULL;
    		// argv[];
    	}

    	// printf("argv: ..%s.. - ..%s.. - ..%s..", argv[0], argv[1], argv[2]);


		// TODO
		// Add code to spawn processes for the first 9 commands
		// And add code to execute cd, exit, help commands
		// Use the example provided in myspawn.c

//char *allowed[N] = {"cp","touch","mkdir","ls","pwd","cat","grep","chmod","diff","cd","exit","help"};

    	if (strcmp(cmd, "cp") == 0) {
    		if (argc == 3) {
    			argv[argc] = NULL;
    			spawn(argv);
    		} else {
    			printf("Too many args");
    		}

    	}
    	if (strcmp(line, "touch") == 0) {
			// spawn(argv);
    	}
    	if (strcmp(line, "mkdir") == 0) {
    		// spawn(argv);
    	}
    	if (strcmp(cmd, "ls") == 0) {
    		argv[argc] = NULL;
    		spawn(argv);
    	}
    	if (strcmp(cmd, "pwd") == 0) {
    		argv[argc] = NULL;
    		spawn(argv);
    	}
    	if (strcmp(line, "cat") == 0) {
    		// spawn(argv);
    	}
    	if (strcmp(line, "grep") == 0) {
    		// spawn(argv);
    	}
    	if (strcmp(line, "chmod") == 0) {
    		// spawn(argv);
    	}
    	if (strcmp(line, "diff") == 0) {
    		// spawn(argv);
    	}
    	if (strcmp(cmd, "cd") == 0) {
    		if (argc > 2) {
    			printf("-rsh: cd: too many arguments");
    		} else chdir(argv[1]);
    		// spawn(argv);
    	}



    	if (strcmp(line, "help") == 0) {
    		printf("The allowed commands are:\n");
    		printf("cp\ntouch\nmkdir\nls\npwd\ncat\ngrep\nchmod\ndiff\ncd\nexit\nhelp\n");
    	}

    	if (strcmp(line, "exit") == 0) {
			return 0;
		}

    	free(cmd);

    }
    return 0;
}
