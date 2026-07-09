CC = gcc
CFLAGS = -Wall -Wextra -g

BIN_DIR = bin
BUILD_DIR = build

all: $(BIN_DIR)/udp_sender $(BIN_DIR)/udp_receiver

$(BIN_DIR)/udp_sender: $(BUILD_DIR)/udp_sender.o
	$(CC) $(CFLAGS) -o $(BIN_DIR)/udp_sender $(BUILD_DIR)/udp_sender.o

$(BIN_DIR)/udp_receiver: $(BUILD_DIR)/udp_receiver.o
	$(CC) $(CFLAGS) -o $(BIN_DIR)/udp_receiver $(BUILD_DIR)/udp_receiver.o

$(BUILD_DIR)/udp_sender.o: networking/udp_sender.c
	$(CC) $(CFLAGS) -c networking/udp_sender.c -o $(BUILD_DIR)/udp_sender.o

$(BUILD_DIR)/udp_receiver.o: networking/udp_receiver.c
	$(CC) $(CFLAGS) -c networking/udp_receiver.c -o $(BUILD_DIR)/udp_receiver.o

clean:
	rm -f $(BUILD_DIR)/*.o $(BIN_DIR)/udp_sender $(BIN_DIR)/udp_receiver