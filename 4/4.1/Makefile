CC = gcc
CFLAGS = -Wall -Wextra -Iunity -Isrc -g

SRC_DIR = src
TEST_DIR = tests
UNITY_DIR = unity
BIN_DIR = bin

SRC_FILES = $(filter-out $(SRC_DIR)/main.c, $(wildcard $(SRC_DIR)/*.c))
MAIN_FILE = $(SRC_DIR)/main.c

UNITY_SRC = $(UNITY_DIR)/unity.c

TEST_FILES = $(wildcard $(TEST_DIR)/test_*.c)

TEST_TARGETS = $(patsubst $(TEST_DIR)/%.c, $(BIN_DIR)/%, $(TEST_FILES))

TARGET = $(BIN_DIR)/app

$(shell mkdir -p $(BIN_DIR))

all: $(TARGET)

$(TARGET): $(SRC_FILES) $(MAIN_FILE)
	$(CC) $(CFLAGS) -o $@ $^

$(BIN_DIR)/%: $(TEST_DIR)/%.c $(SRC_FILES) $(UNITY_SRC)
	$(CC) $(CFLAGS) -o $@ $^

.PHONY: test
test: $(TEST_TARGETS)
	@echo "Running all tests..."
	@for test in $(TEST_TARGETS); do \
		echo "==== Running $$test ===="; \
		./$$test || exit 1; \
	done

.PHONY: clean
clean:
	rm -rf $(BIN_DIR)/*