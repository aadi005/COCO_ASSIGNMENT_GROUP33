# Group 33
# Aaditya Goel 2022B3A70417P
# Divyansh Jha 2022B3A70438P
# Prakhar Mittal 2022B3A70426P
# Mukund Srivastava 2022B3A70562P
# Aryan Gupta 2022B3A70495P
# Garv Gulati 2022B4A70685P
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

TARGET = $(TARGET_NAME)$(EXE)
SRC_DIR = src
BUILD_DIR = build

# All source files
SRCS = $(wildcard $(SRC_DIR)/*.c)

# Object files inside build/
OBJS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))

all: directories $(TARGET)

# Create build directory if it doesn't exist
directories:
	$(MKDIR) $(BUILD_DIR)

# Link object files to executable in project root
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

# Compile src/file.c → build/file.o
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean everything
clean:
	$(RMRF) $(BUILD_DIR)
	$(RMRF) $(TARGET)

.PHONY: all clean directories
