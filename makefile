# =========================================
# Compiler Construction Project Makefile
# =========================================

CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g

TARGET = stage1exe
SRC_DIR = src

# Automatically pick all .c files inside src
SRCS = $(wildcard $(SRC_DIR)/*.c)

# Generate corresponding object files
OBJS = $(SRCS:.c=.o)

# Default target
all: $(TARGET)

# Link all object files
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Generic rule to compile .c → .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build files
clean:
	rm -f $(TARGET) $(OBJS)