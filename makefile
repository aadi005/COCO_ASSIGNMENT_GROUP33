# =========================================
# Compiler Construction Project Makefile
# =========================================

CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g

TARGET = stage1exe

# Source directory
SRC_DIR = src

# Source files $(SRC_DIR)/parser.c
SRCS = $(SRC_DIR)/driver.c \
       $(SRC_DIR)/lexer.c \
	   $(SRC_DIR)/stateHandlers.c

# Object files (generated in root directory) parser.o
OBJS = driver.o lexer.o stateHandlers.o 


# Default target
all: $(TARGET)


# Link object files
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)


# Compile driver $(SRC_DIR)/parser.h
driver.o: $(SRC_DIR)/driver.c $(SRC_DIR)/lexer.h $(SRC_DIR)/stateHandlers.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/driver.c -o driver.o

# Compile lexer
lexer.o: $(SRC_DIR)/lexer.c $(SRC_DIR)/lexerDef.h 
	$(CC) $(CFLAGS) -c $(SRC_DIR)/lexer.c -o lexer.o

stateHandlers.o: $(SRC_DIR)/stateHandlers.c $(SRC_DIR)/stateHandlers.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/stateHandlers.c -o stateHandlers.o


# # Compile parser
# parser.o: $(SRC_DIR)/parser.c $(SRC_DIR)/parserDef.h $(SRC_DIR)/lexer.h
# 	$(CC) $(CFLAGS) -c $(SRC_DIR)/parser.c -o parser.o


# Clean
clean:
	rm -f $(OBJS) $(TARGET)