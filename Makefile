CC = gcc
CFLAGS = -Wall -Wextra -g

BIN_DIR = bin
BUILD_DIR = build

SRC := $(wildcard sandbox/networking/*.c)
BINS := $(patsubst sandbox/networking/%.c, $(BIN_DIR)/%, $(SRC))

all: $(BINS)

$(BIN_DIR)/%: $(BUILD_DIR)/%.o
	$(CC) $(CFLAGS) -o $@ $<

$(BUILD_DIR)/%.o: sandbox/networking/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(BUILD_DIR)/*.o $(BIN_DIR)/*