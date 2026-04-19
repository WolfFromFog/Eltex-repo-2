SRC_CLIENT = client.c
SRC_SERVER = server.c

TARGET_SRV = server
TARGET_CLI = client

all: ${TARGET_CLI} ${TARGET_SRV}

${TARGET_CLI}: ${SRC_CLIENT}
	gcc -Wall ${SRC_CLIENT} -o ${TARGET_CLI}

${TARGET_SRV}: ${SRC_SERVER}
	gcc -Wall ${SRC_SERVER} -o ${TARGET_SRV}

