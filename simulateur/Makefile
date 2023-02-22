CC = g++
CFLAGS = -Wall -std=c++11
SRC_DIR = .
OBJ_DIR = obj

# List of source files
SRCS = $(SRC_DIR)/iss.c \
       $(SRC_DIR)/memory/memory.cpp \
	   $(SRC_DIR)/cache/cache.cpp


# Generate names of object files
OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# Name of the executable file
EXEC = iss

.PHONY: all clean

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(OBJ_DIR)/memory \
	         $(OBJ_DIR)/cache

	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(EXEC)