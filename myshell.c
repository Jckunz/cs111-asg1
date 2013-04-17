//myshell.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>

extern char **get_line();

main() {
	int i;
	char **args; 
	while(1) {
		printf("myShell>");
		args = get_line();
		if(strcmp(args[0], "exit") == 0) exit(0);
		for(i = 0; args[i] != NULL; i++) {
			printf("Argument %d: %s\n", i, args[i]);
		}
	}
}
