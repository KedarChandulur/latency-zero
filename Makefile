CC = gcc
CFLAGS = -Wall -Wextra -g

BIN_DIR = bin
BUILD_DIR = build

# List all available scripts here
SCRIPTS := udp_sender udp_receiver tcp_sender tcp_receiver

all: $(patsubst %, $(BIN_DIR)/%, $(SCRIPTS))

$(BIN_DIR)/%: $(BUILD_DIR)/%.o
	$(CC) $(CFLAGS) -o $@ $<

$(BUILD_DIR)/%.o: networking/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(BUILD_DIR)/*.o $(BIN_DIR)/*