phonebook: main.c phonebook.c phonebook.h
	gcc -Wall -std=c99 -g -o phonebook main.c phonebook.c

clean:
	rm -f phonebook

run: phonebook
	./phonebook

.PHONY: clean run