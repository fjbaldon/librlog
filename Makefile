all: $(BIN_DIR)/$(BIN_NAME)


CC = gcc
SRC_DIR = src
BIN_DIR = bin
BIN_NAME = librlog
WARNINGS = -Wall
DEBUG = -ggdb -fno-omit-frame-pointer
OPTIMIZE = -O2


$(BIN_DIR)/$(BIN_NAME): Makefile $(wildcard $(SRC_DIR)/*.c) | $(BIN_DIR)
	$(CC) -o $@ $(WARNINGS) $(DEBUG) $(OPTIMIZE) $(wildcard $(SRC_DIR)/*.c)

clean:
	rm -f $(BIN_DIR)/$(BIN_NAME)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

all: clean $(BIN_DIR)/$(BIN_NAME)

# Builder will call this to install the application before running.
install:
	echo "Installing is not supported"

# Builder uses this target to run your application.
run: $(BIN_DIR)/$(BIN_NAME)
	./$(BIN_DIR)/$(BIN_NAME)
 
