# executable "myshell"
myshell: clean LineParser.o myshell.o
	gcc -m32 -g -Wall -o myshell myshell.o LineParser.o

# myshell.c
myshell.o: myshell.c
	gcc -m32 -g -Wall -ansi -c -o myshell.o myshell.c

# LineParser.c
LineParser.o: LineParser.c LineParser.h
	gcc -m32 -g -Wall -ansi -c -o LineParser.o LineParser.c

clean:
	rm -f *.o myshell
