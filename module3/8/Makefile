TARGET_PROD = producer/producer
TARGET_CONS = consumer/consumer

SRC_RPOD = producer/main.c producer/producer.c
SRC_CONS = consumer/consumer.c consumer/main.c

all: ${TARGET_CONS} ${TARGET_PROD}

${TARGET_PROD}: ${SRC_RPOD}
	gcc -Wall ${SRC_RPOD} -o ${TARGET_PROD}

${TARGET_CONS}: ${SRC_CONS}
	gcc -Wall ${SRC_CONS} -o ${TARGET_CONS}