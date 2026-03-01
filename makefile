# =========================================
# Compiler Construction Project Makefile
# =========================================

CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g

TARGET = bin/stage1exe
SRC_DIR = src
BUILD_DIR = build
BIN_DIR = bin

# All source files
SRCS = $(wildcard $(SRC_DIR)/*.c)

# Object files inside build/
OBJS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))

# Default target
all: directories $(TARGET)

# Create build and bin directories if they don't exist
directories:
	mkdir -p $(BUILD_DIR)
	mkdir -p $(BIN_DIR)

# Link object files → executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

# Compile src/file.c → build/file.o
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean everything
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)