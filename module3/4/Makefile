all: interpreter functions

interpreter: interpreter.h interpreter.c main.c
	gcc -Wall interpreter.c main.c -o interpreter

functions:
	gcc -Wall sum.c -o sum

clean:
	rm -f interpreter sum
	
run: interpreter functions
	./interpreter ./functions

.PHONY: clean run functions