all: phonebook

libphonebook.a: phonebook.o
	ar rcs libphonebook.a phonebook.o

phonebook.o: phonebook.c phonebook.h
	gcc -Wall -std=c99 -g -c phonebook.c

phonebook: main.c libphonebook.a
	gcc -Wall -std=c99 -g -o phonebook main.c -L. -lphonebook

clean:
	rm -f phonebook *.o *.a

run: phonebook
	./phonebook

.PHONY: clean run all