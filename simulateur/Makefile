SRC_DIR  = .
OBJ_DIR  = ${SRC_DIR}/obj
MEMORY_DIR = ${SRC_DIR}/memory
CACHE_DIR = ${SRC_DIR}/cache

SRC_ISS    = ${SRC_DIR}/iss.c
SRC_MEMORY = ${MEMORY_DIR}/memory.cpp
SRC_CACHE  = ${CACHE_DIR}/cache.cpp

OBJ_MEMORY = ${OBJ_DIR}/memory/memory.o 
OBJ_CACHE  = ${OBJ_DIR}/cache/cache.o 

EXEC = ${SRC_DIR}/iss

CC       = g++
CFLAGS   = -Wall -std=c++11
CPPFLAGS = -I${MEMORY_DIR} -I${CACHE_DIR}


all: ${EXEC}

${EXEC}: ${SRC_ISS} ${OBJ_MEMORY} ${OBJ_CACHE}
	${CC} ${CFLAGS} ${CPPFLAGS} $^ -o $@

${OBJ_MEMORY}: ${SRC_MEMORY}
	mkdir -p ${OBJ_DIR}/memory 
	${CC} ${CFLAGS} ${CPPFLAGS} -c $< -o $@

${OBJ_CACHE}: ${SRC_CACHE}
	mkdir -p ${OBJ_DIR}/cache
	${CC} ${CFLAGS} ${CPPFLAGS} -c $< -o $@

clean:
	rm -rf ${OBJ_DIR} ${EXEC}