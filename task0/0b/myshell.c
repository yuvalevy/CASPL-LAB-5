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

  switch(fork_res) {

   case -1 :  /* fork faliure */
      perror("fork failed\n");
      break;   
   case 0 : /* child, executing the command */
   
      printf("%d: I'm child\n", getpid());
      execution_result = execvp(line->arguments[0], line->arguments);
      printf("%d: finished running %s\n", getpid(), line->arguments[0]);
      break;
	
   default : /* father, waiting for child to finish */
      printf("%d: I'm papi\n", getpid());
      waitpid(fork_res, NULL, WUNTRACED);
      printf("%d: finished wating to %d\n", getpid(), fork_res);
      break; 
  }
  
  if ( -1 == execution_result ) {
      perror("execution failed");
      exit(1);
  }
}

void handle_signal(int signal) {

  /*char* signal_name = strsignal(signal);*/
  printf("%d: The signal %d was ignored by mysheel\n", getpid(), signal);
  
}

int main (int argc, char* argv[]) {

	char* curDir = NULL;
	char user_input[2048] = {0};
  cmdLine* line = NULL;
  curDir = getcwd(curDir, PATH_MAX);

  signal(SIGCHLD, handle_signal);
  /*signal(SIGQUIT, handle_signal);*/
  
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


