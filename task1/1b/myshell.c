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
int debug = 0;

void print_debug(char * arg) {
    
    if(debug == 1) {
        fprintf(stderr, "%d: running %s\n", getpid() ,arg);
    }

}

void execute(cmdLine* line) {

  pid_t fork_res = fork();

  print_debug(line->arguments[0]);
  
  switch(fork_res) {

   case -1 :  /* fork faliure */
       
      perror("fork failed\n");
      _exit(1);
      break;   
      
   case 0 : /* child, executing the command */
   
       execvp(line->arguments[0], line->arguments);   
       perror("execution failed");
       _exit(1);
   break;
	
   default : /* father, waiting for child to finish */
       if(line->blocking == 1) {
           waitpid(fork_res, NULL, WUNTRACED);
       }
      break; 
  }
}

void handle_signal(int signal_num) {

  char* signal_name = strsignal(signal_num);
  printf("%d: The signal %s was ignored by mysheel\n", getpid(), signal_name);
  
}

int main(int argc, char* argv[]) {

    char* curDir = NULL;
    char user_input[2048] = {0};
    cmdLine* line = NULL;
    
    if (argc > 1) {
     
        if(0==strcmp(argv[1],"-d")) {
            debug = 1;
        }
    }
    
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
        if (0 == strcmp(user_input, "\n")) {
            continue;
        }
        line = parseCmdLines(user_input);

        /*printf("line: \n");
        for ( i = 0 ; i < line->argCount ; i++ ) {
            printf("arg[%d]: %s\n", i, line->arguments[i]);
        }
        printf("inputRedirect: %s\n", line->inputRedirect);
        printf("outputRedirect: %s\n", line->outputRedirect);        
        if((line->next)!=NULL) {
            printf("next: %s\n", line->next->arguments[0]);
        }*/
        execute(line);
        free(line);
    }
    return 0;
}


