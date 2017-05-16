# executable "myshell"
myshell: clean JobControl.o LineParser.o myshell.o
	gcc -m32 -g -Wall -o myshell myshell.o LineParser.o JobControl.o

# myshell.c
myshell.o: myshell.c
	gcc -m32 -g -Wall -ansi -c -o myshell.o myshell.c

# LineParser.c
LineParser.o: LineParser.c LineParser.h
	gcc -m32 -g -Wall -ansi -c -o LineParser.o LineParser.c

# JobControl.c
JobControl.o: JobControl.c JobControl.h
	gcc -m32 -g -Wall -ansi -c -o JobControl.o JobControl.c

clean:
	rm -f *.o myshell
