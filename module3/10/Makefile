TARGET_PROD = producer
TARGET_CONS = consumer

SRC_RPOD = factory.c producer.c
SRC_CONS = factory.c consumer.c

all: ${TARGET_CONS} ${TARGET_PROD}

${TARGET_PROD}: ${SRC_RPOD}
	gcc -Wall ${SRC_RPOD} -o ${TARGET_PROD}

${TARGET_CONS}: ${SRC_CONS}
	gcc -Wall ${SRC_CONS} -o ${TARGET_CONS}