interpreter: interpreter.h interpreter.c main.c
	gcc -Wall interpreter.c main.c -o interpreter

clean:
	rm -f interpreter 
	
run: interpreter 
	./interpreter 

.PHONY: clean run