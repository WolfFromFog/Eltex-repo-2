all: phonebook

libphonebook.so: phonebook.o
	gcc -shared -o libphonebook.so phonebook.o

phonebook.o: phonebook.c phonebook.h
	gcc -Wall -std=c99 -fPIC  -g -c phonebook.c

phonebook: main.c libphonebook.so
	gcc -Wall -std=c99 -g -o phonebook main.c -L. -lphonebook -Wl,-rpath,.

clean:
	rm -f phonebook *.o *.a

run: phonebook
	./phonebook

.PHONY: clean run all