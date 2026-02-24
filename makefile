# =========================================
# Compiler Construction Project Makefile
# =========================================

CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g

# Executable name
TARGET = stage1exe

# Source files parser.c
SRCS = driver.c lexer.c 

# Object files parser.o
OBJS = driver.o lexer.o 

# Default target
all: $(TARGET)

# Link object files to create executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Compile driver parser.h
driver.o: driver.c lexer.h 
	$(CC) $(CFLAGS) -c driver.c

# Compile lexer
lexer.o: lexer.c lexerDef.h
	$(CC) $(CFLAGS) -c lexer.c

# Compile parser
# parser.o: parser.c parserDef.h lexer.h
# 	$(CC) $(CFLAGS) -c parser.c

# Clean generated files
clean:
	rm -f $(OBJS) $(TARGET)