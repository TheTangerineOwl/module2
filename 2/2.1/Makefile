CC = gcc
CFLAGS = -Wall -Wextra -Iunity -Isrc

SRC_DIR = src
TEST_DIR = tests
UNITY_DIR = unity

SRC_FILES = $(SRC_DIR)/*.c
TEST_FILES = $(TEST_DIR)/test_*.c $(UNITY_DIR)/unity.c

TARGET = bin/app
TEST_TARGET = bin/test_runner

$(shell mkdir -p bin)

all: $(TARGET)

$(TARGET): $(SRC_FILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC_FILES)

.PHONY: test
test: $(SRC_FILES) $(TEST_FILES)
	$(CC) $(CFLAGS) -o $(TEST_TARGET) $(SRC_FILES) $(TEST_FILES)
	./$(TEST_TARGET)

.PHONY: clean
clean:
	rm -rf bin/*.o bin/2-1-exec