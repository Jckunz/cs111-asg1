//myshell.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>

void execute(char **, int, char **);
int parse(char **, char **, int *);

#define DEFAULT_MODE         0
#define BACKGROUND_MODE      1
#define OUTPUT_REDIRECT_MODE 2
#define INPUT_REDIRECT_MODE  3
#define PIPELINE_MODE        4

extern char **get_line();

int main(int argc, char *argv[]) {
	int mode = DEFAULT_MODE;
	int commandArgc;
	char *supp = NULL;
	char **args; 
	while(1) {
		printf("myShell>"); //display prompt
		args = get_line();
		if(strcmp(args[0], "exit") == 0) exit(0);
		commandArgc = parse(args, &supp, &mode);
		if(strcmp(args[0], "cd") == 0) chdir(args[1]);
		else execute(args, mode, &supp);
		printf("command arg count: %d\n", commandArgc);
	}
	return 0;
}

int parse(char **input, char **suppPtr, int *modePtr) {
	int i, commandArgc = 0;
	for(i = 0; input[i] != NULL; i++) {
		commandArgc++;
		if (strcmp(input[i], "&") == 0) {
			*modePtr = BACKGROUND_MODE;
			printf("running in Background Mode\n");
		} else if (strcmp(input[i], ">") == 0) {
			*modePtr = OUTPUT_REDIRECT_MODE;
			printf("running in Output Redirect Mode\n");
		} else if (strcmp(input[i], "<") == 0) {
			*modePtr = INPUT_REDIRECT_MODE;
			printf("running in Input Redirect Mode\n");
		} else if (strcmp(input[i], "|") == 0) {
			*modePtr = PIPELINE_MODE;
			printf("running in Pipeline Mode\n");
		}
		printf("Argument %d: %s\n", i, input[i]);
	}
	return commandArgc;
}

void execute(char **command, int mode, char **suppPtr) {
   printf("executing\n");
}
