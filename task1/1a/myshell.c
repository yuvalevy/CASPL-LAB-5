#include "LineParser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <linux/limits.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>

void execute(cmdLine* line) {
  
  int execution_result = 0;
  pid_t fork_res = fork();

  printf("%d: running %s\n", getpid(), line->arguments[0]);
  
  switch(fork_res) {

   case -1 :  /* fork faliure */
       
      perror("fork failed\n");
      execution_result = -1;
      break;   
      
   case 0 : /* child, executing the command */
   
      execution_result = execvp(line->arguments[0], line->arguments);   
      break;
	
   default : /* father, waiting for child to finish */
       
      waitpid(fork_res, NULL, WUNTRACED);
      break; 
  }
  
  if ( -1 == execution_result ) {
      perror("execution failed");
      exit(1);
  }
}

void handle_signal(int signal_num) {

  char* signal_name = strsignal(signal_num);
  printf("%d: The signal %s was ignored by mysheel\n", getpid(), signal_name);
  
}

int main (int argc, char* argv[]) {

    char* curDir = NULL;
    char user_input[2048] = {0};
    cmdLine* line = NULL;
    curDir = getcwd(curDir, PATH_MAX);

    signal(SIGCHLD, handle_signal);
    signal(SIGQUIT, handle_signal);
    signal(SIGTSTP, handle_signal);

    while(1) {
        printf("%s>", curDir);
        fgets(user_input, 2048, stdin);

        if (0 == strcmp(user_input, "quit\n")) {
            return 0;
        }

        line = parseCmdLines(user_input);
        execute(line);
        free(line);
    }
    return 0;
}


