TARGET = pin_pong

SRC = main.c posix_queue.c

all: ${TARGET}

${TARGET}: ${SRC}
	gcc -Wall ${SRC} -o ${TARGET} -lrt

.PHONY: all

