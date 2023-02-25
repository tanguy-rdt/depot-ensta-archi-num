SRC_DIR  = .
OBJ_DIR  = ${SRC_DIR}/obj

SRC_ISS    = ${SRC_DIR}/iss.c
SRC_MEMORY = ${SRC_DIR}/memory/memory.cpp
SRC_CACHE  = ${SRC_DIR}/cache/cache.cpp

OBJ_MEMORY = ${OBJ_DIR}/memory/memory.o 
OBJ_CACHE  = ${OBJ_DIR}/cache/cache.o 

EXEC = ${SRC_DIR}/iss

CC       = g++
CFLAGS   = -Wall -std=c++11
CPPFLAGS = -I${OBJ_MEMORY} -I${OBJ_CACHE}


all: ${EXEC}

${EXEC}: ${SRC_ISS} ${OBJ_MEMORY} ${OBJ_CACHE}
	echo ${SRC_DIR}
	${CC} ${CFLAGS} $^ -o $@

${OBJ_MEMORY}: ${SRC_MEMORY}
	mkdir -p ${OBJ_DIR}/memory 
	${CC} ${CFLAGS} -c $< -o $@

${OBJ_CACHE}: ${SRC_CACHE}
	mkdir -p ${OBJ_DIR}/cache
	${CC} ${CFLAGS} -c $< -o $@

clean:
	rm -rf ${OBJ_DIR} ${EXEC}