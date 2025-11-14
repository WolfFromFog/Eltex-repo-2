hello: hello.c
	gcc -Wall hello.c -o hello
clean:
	rm -f hello exec_test
run: hello exec_test
	./hello
	./exec_test

.PHONY: clean run
