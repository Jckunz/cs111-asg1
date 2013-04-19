#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>

void execute(char **, int, char **);
int parse(char **, char *[], char **, int *);

#define DEFAULT_MODE         0
#define BACKGROUND_MODE      1
#define OUTPUT_REDIRECT_MODE 2
#define INPUT_REDIRECT_MODE  3

extern char **get_line();

int main(int argc, char *argv[])
{
  int mode = DEFAULT_MODE;
  int command_argc;
  char *command[80], *sup = NULL;
  char **args;

  while (1) {
    printf("myShell>"); //display prompt
    args = get_line();
    if (strcmp(args[0], "exit") == 0) exit(0);
    command_argc = parse(args, command, &sup, &mode);
    if (strcmp(args[0], "cd") == 0) chdir(args[1]);
    else execute(args, mode, &sup);
    //printf("command arg count: %d\n", commandArgc);
  }
  return 0;
}

int parse(char **input, char *command[], char **sup_ptr, int *mode_ptr)
{
  int i, command_argc = 0;
  int terminate = 0;
  *sup_ptr = NULL;
  command = input;
  for (i = 0; input[i] != NULL && !terminate; i++) {
    command_argc++;
    if (strcmp(input[i], "&") == 0) {
      *mode_ptr = BACKGROUND_MODE;
      //printf("running in Background Mode\n");
    } else if (strcmp(input[i], ">") == 0) {
      *mode_ptr = OUTPUT_REDIRECT_MODE;
      *sup_ptr = input[i+1];
      command[i] = '\0';
      terminate = 1;
      //printf("running in Output Redirect Mode\n");
    } else if (strcmp(input[i], "<") == 0) {
      *mode_ptr = INPUT_REDIRECT_MODE;
      *sup_ptr = input[i+1];
      command[i] = '\0';
      terminate = 1;
      //printf("running in Input Redirect Mode\n");
    } 
  }
  command[i] = '\0';
  return command_argc;
}

void execute(char **command, int mode, char **sup_ptr)
{
  pid_t pid1, pid2;
  FILE *file;
  char *sup2 = NULL;
  char *command2[80];
  int command_argc2;
  int mode2 = DEFAULT_MODE, pid1_status, pid2_status;
  int pipeline[2];
  int n;
  if (*sup_ptr != NULL) printf("Supplement: %s\n", *sup_ptr);
  for (n = 0; command[n] != NULL; n++) {
    printf("command[%d]: %s\n", n, command[n]);
  }
  pid1 = fork();
  if (pid1 < 0) {
    printf("Error: pid1 < 0\n");
    exit(1);
  } else if (pid1 == 0) {
    switch (mode) {
      case OUTPUT_REDIRECT_MODE:
        file = freopen(*sup_ptr, "w+", stdout);
        dup2(fileno(file), fileno(stdout));
        break;
      case INPUT_REDIRECT_MODE:
        file = freopen(*sup_ptr, "r", stdin);
        dup2(fileno(file), fileno(stdin));
        break;
    }
    execvp(*command, command);
  } else {
    if (mode == BACKGROUND_MODE);
    else waitpid(pid1, &pid1_status, 0);
  }
}
