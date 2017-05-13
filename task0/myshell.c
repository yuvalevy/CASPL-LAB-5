#include "LineParser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <linux/limits.h>

void execute(cmdLine* line) {

  int execution_result = execvp(line->arguments[0], line->arguments);
  if ( -1 == execution_result ) {
      perror("execution failed");
  }
}

int main (int argc, char* argv[]) {

	char* curDir = NULL;
	char user_input[2048] = {0};
  cmdLine* line = NULL; 
	
  curDir = getcwd(curDir, PATH_MAX);
  
    while(1) {
  		printf("%s>", curDir);
  		fgets(user_input, 2048, stdin);
  
  		if (0 == strcmp(user_input, "quit\n")) {
          return 1;
      }
  
   		line = parseCmdLines(user_input);
      execute(line);
      printf("finish 1\n");
  		free(line);
      printf("finish 2\n");
    }
	return 0;
}


