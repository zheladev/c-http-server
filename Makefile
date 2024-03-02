CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c11
LDFLAGS =

SRC_DIR = src
INC_DIR = include
BUILD_DIR = build

# List all source files (excluding main.c)
SRC_FILES := $(wildcard $(SRC_DIR)/**/*.c $(SRC_DIR)/*.c)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC_FILES))

EXECUTABLE = http_server

.PHONY: all clean

all: $(BUILD_DIR) $(EXECUTABLE)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(EXECUTABLE): $(OBJ_FILES)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(EXECUTABLE)