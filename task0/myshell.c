#include <stdio.h>
#include <unistd.h>
#include <linux/limits.h>

int main (int argc, char* argv[]) {

	char* curDir = NULL;
	
	curDir = getwd(curDir, PATH_MAX);
	
		printf("%s>", curDir);
}