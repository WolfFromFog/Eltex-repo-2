SRC_CLIENT = client.c
SRC_SERVER = server.c
SRCS = networker.c


TARGET_SRV = server
TARGET_CLI = client

all: ${TARGET_CLI} ${TARGET_SRV}

${TARGET_CLI}: ${SRC_CLIENT} ${SRCS}
	gcc -Wall ${SRC_CLIENT} ${SRCS} -o ${TARGET_CLI}

${TARGET_SRV}: ${SRC_SERVER} ${SRCS}
	gcc -Wall ${SRC_SERVER} ${SRCS} -o ${TARGET_SRV}

