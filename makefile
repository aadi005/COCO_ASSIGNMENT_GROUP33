# =========================================
# Compiler Construction Project Makefile
# =========================================

CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g

TARGET_NAME = stage1exe
# On Windows add .exe extension when needed
ifeq ($(OS),Windows_NT)
EXE := .exe
MKDIR := mkdir
RMRF := rmdir /S /Q
else
EXE :=
MKDIR := mkdir -p
RMRF := rm -rf
endif

TARGET = $(BIN_DIR)/$(TARGET_NAME)$(EXE)
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
	$(MKDIR) $(BUILD_DIR)
	$(MKDIR) $(BIN_DIR)

# Link object files → executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

# Compile src/file.c → build/file.o
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean everything
clean:
	$(RMRF) $(BUILD_DIR)
	$(RMRF) $(BIN_DIR)

.PHONY: all clean directories