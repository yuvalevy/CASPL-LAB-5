#include <LineParser.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <linux/limits.h>

int main (int argc, char* argv[]) {

	char* curDir = NULL;
	char* user_input = NULL;
	cmdLine* line = NULL;
	
	curDir = getcwd(curDir, PATH_MAX);
	
	while (0) {
		
		printf("%s>", curDir);
		fgets(user_input, 2048, stdin);
		
		if (strcmp(user_input, "quit")) {
			break;
		}
		
		line = parseCmdLines(user_input);
		printf("%s/n", line->arguments);
		execute(line);
		free(line);
	}
	return 0;
}

void execute(cmdLine* pCmdLine) {
	
	
	
}