#include "LineParser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <linux/limits.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "JobControl.h"

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
       
      print_debug("fork failed\n");
      _exit(1);
      break;   
      
   case 0 : /* child, executing the command */
   
       execvp(line->arguments[0], line->arguments);   
       print_debug("execution failed");
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

void change_directory(char* command) {

    int len = strlen(command) - 3 -1 ; /* -3 for "cd " and -1 to \n */
    char* dir = malloc(len);
    strncpy(dir, command+3 , len);
    
    if(chdir(dir) == -1){
        print_debug("cd failed");
    }

}

int is_cd_command(char* dir) {
 
    if( strlen(dir) < 2 ) {
        return 1;
    }
    
    return strncmp(dir, "cd", 2);
}

int main(int argc, char* argv[]) {

    char* curDir = NULL;
    char user_input[2048] = {0};
    cmdLine* line = NULL;
    job* job_list = NULL;
    
    if (argc > 1) {
     
        if (0 == strcmp(argv[1],"-d")) {
            debug = 1;
        }
    }
    
    signal(SIGCHLD, handle_signal);
    signal(SIGQUIT, handle_signal);
    signal(SIGTSTP, handle_signal);

    while(1) {
        
        curDir = getcwd(curDir, PATH_MAX);
        printf("%s>", curDir);
        fgets(user_input, 2048, stdin);

        if (0 == strcmp(user_input, "quit\n")) {
            return 0;
        } 
       
        if (0 == strcmp(user_input, "\n")) {
            continue;
        } 

        if (0 == is_cd_command(user_input)) {
            change_directory(user_input);
            continue;
        }

        if (0 == strcmp(user_input, "jobs\n")) {
            printJobs(&job_list);
            continue;
        }

        addJob(&job_list, user_input)->status = 1;
        
        line = parseCmdLines(user_input);
        execute(line);
        free(line);
        
    }
    
    freeJobList(&job_list);
    
    return 0;
}


/*printf("line: \n");
        for ( i = 0 ; i < line->argCount ; i++ ) {
            printf("arg[%d]: %s\n", i, line->arguments[i]);
        }
        printf("inputRedirect: %s\n", line->inputRedirect);
        printf("outputRedirect: %s\n", line->outputRedirect);        
        if((line->next)!=NULL) {
            printf("next: %s\n", line->next->arguments[0]);
        }*/