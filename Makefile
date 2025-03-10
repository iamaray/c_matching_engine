CC = gcc
CFLAGS = -Wall -Wextra -g -I$(SRC_DIR) -I. -Iinclude -I$(SRC_DIR)/core
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

# Source files
SRCS = $(shell find $(SRC_DIR) -name "*.c")
# Object files
OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))
# Executable name
EXEC = $(BIN_DIR)/orderbook

# Test files
TEST_DIR = tests
TEST_SRCS = $(wildcard $(TEST_DIR)/*.c)
TEST_OBJS = $(TEST_SRCS:$(TEST_DIR)/%.c=$(OBJ_DIR)/%.o)
TEST_EXECS = $(TEST_SRCS:$(TEST_DIR)/%.c=$(BIN_DIR)/%)

# Default target
all: directories $(EXEC)
tests: directories $(TEST_EXECS)

# Create necessary directories
directories:
	mkdir -p $(OBJ_DIR) $(BIN_DIR)
	@mkdir -p $(dir $(OBJS))

# Compile source files to object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Compile test files
$(OBJ_DIR)/%.o: $(TEST_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Link object files to create executable
$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

# Link test executables
$(BIN_DIR)/test_%: $(OBJ_DIR)/test_%.o $(filter-out $(OBJ_DIR)/main.o, $(OBJS))
	$(CC) $(CFLAGS) $^ -o $@

# Clean up
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# Run the program
run: all
	./$(EXEC)

# Run tests
run_tests: tests
	for test in $(TEST_EXECS); do ./$$test; done

# Phony targets
.PHONY: all clean run directories tests run_tests
