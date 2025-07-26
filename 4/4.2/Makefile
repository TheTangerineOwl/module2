CC = gcc
CFLAGS = -Wall -Wextra -Iunity -Isrc

SRC_DIR = src
TEST_DIR = tests
UNITY_DIR = unity

SRC_FILES = $(filter-out $(SRC_DIR)/main.c, $(wildcard $(SRC_DIR)/*.c))
MAIN_FILE = $(SRC_DIR)/main.c
TEST_FILES = $(TEST_DIR)/test_*.c $(UNITY_DIR)/unity.c

TARGET = bin/app
TEST_TARGET = bin/test_runner

$(shell mkdir -p bin)

all: $(TARGET)

$(TARGET): $(SRC_FILES) $(MAIN_FILE)
	$(CC) $(CFLAGS) -o $(TARGET) $^

.PHONY: test
test: $(SRC_FILES) $(TEST_FILES)
	$(CC) $(CFLAGS) -o $(TEST_TARGET) $^
	./$(TEST_TARGET)

.PHONY: clean
clean:
	rm -rf $(TARGET) $(TEST_TARGET)