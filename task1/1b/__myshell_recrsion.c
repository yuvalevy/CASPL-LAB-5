#include "LineParser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <linux/limits.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>

char* strsignal(int);
void execute(cmdLine*);
    
void execute_waitpid(cmdLine* line, pid_t wait_id) {
    
    pid_t fork_res = fork();
    
    switch(fork_res) {
     
        case -1 :  /* fork faliure */
       
            perror("fork failed\n");
            exit(1);
            
        break;   
      
        case 0 : /* child, executing the command */
         
            waitpid(wait_id, NULL, WUNTRACED);
            execute(line);
                        
        break;
	
        /*default : father */           
        
    }    
}

void execute(cmdLine* line) {
  
  pid_t fork_res = fork();

  printf("%d: running %s\n", getpid(), line->arguments[0]);
  
  switch(fork_res) {

   case -1 :  /* fork faliure */
       
      perror("fork failed\n");
      exit(1);
   break;   
      
   case 0 : /* child, executing the command */
   
      if (line->next) {
           
           printf("%d: %s has child and his name is %s", getpid(), line->arguments[0], line->next->arguments[0]);
           execute_waitpid(line->next, getpid());
      } 
       
      execvp(line->arguments[0], line->arguments);
      
      perror("execution failed");
      exit(1);
       
   break;
	
   default : /* father, waiting for child to finish */

       if ( 1 == line->blocking ) { /* block until child finishes */
          waitpid(fork_res, NULL, WUNTRACED);
       }
       
   break; 
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


