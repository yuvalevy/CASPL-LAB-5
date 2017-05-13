#include "LineParser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <linux/limits.h>


void exec_line(cmdLine* line) {

}

int main (int argc, char* argv[]) {

	char* curDir = NULL;
	char user_input[2048] = {0};
	int i = 0;
  cmdLine* line = NULL; 
	
  curDir = getcwd(curDir, PATH_MAX);
  
    while(1) {
  		printf("%s>", curDir);
  		fgets(user_input, 2048, stdin);
  
  		if (0 == strcmp(user_input, "quit\n")) {
          return 1;
      }
  
   		line = parseCmdLines(user_input);
      exec_line(line);
      for ( i = 0 ; i< line->argCount ; i++ ) {
       		printf("arguments[%d]: %s \n", i, line->arguments[i]);
      }
  		free(line);
    }
	return 0;
}


