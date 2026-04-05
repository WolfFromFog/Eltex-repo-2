TARGET_SERVER = server/server
TARGET_CLIENT = client/client
SRCS_SERVER = server/server.c server/main.c
SRCS_CLIENT = client/client.c client/main.c
#OBJS_1 = ${SRCS_1:.c=.o}
#OBJS_2 = ${SRCS_2:.c=.o}

all: ${TARGET_SERVER} ${TARGET_CLIENT}

${TARGET_SERVER}: ${SRCS_SERVER}
	gcc -Wall ${SRCS_SERVER} -o ${TARGET_SERVER}

${TARGET_CLIENT}: ${SRCS_CLIENT}
	gcc -Wall ${SRCS_CLIENT} -o ${TARGET_CLIENT}

#clean: 
#	rm -f ./server/${OBJS_1} ./client/${OBJS_2}

.PHONY: all #clean