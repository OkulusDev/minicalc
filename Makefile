CPP=clang++

# Директории
SRC_DIR=src
BIN_DIR=bin

build:
	mkdir -p $(SRC_DIR)
	mkdir -p $(BIN_DIR)
	$(CPP) $(SRC_DIR)/minicalc.cpp -o $(BIN_DIR)/minicalc.bin
