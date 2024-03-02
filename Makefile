CC = gcc
CFLAGS = -Wall -Wextra -pedantic
LDFLAGS =

SRC_DIR = src
INC_DIR = include
BUILD_DIR = build

# List all source files (excluding main.c)
SRC_FILES := $(wildcard $(SRC_DIR)/**/*.c $(SRC_DIR)/*.c)

EXECUTABLE = http_server

.PHONY: all clean

all: $(BUILD_DIR) $(EXECUTABLE)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(EXECUTABLE):
	$(CC) -I $(INC_DIR) -o $(BUILD_DIR)/$(EXECUTABLE) $(SRC_FILES)

clean:
	rm -rf $(BUILD_DIR) $(EXECUTABLE)