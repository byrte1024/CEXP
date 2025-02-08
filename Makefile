# Define paths
RAYLIB_DIR = C:/raylib
RAYLIB_INCLUDE = $(RAYLIB_DIR)/src
RAYLIB_LIB = ./lib/libraylib.a

# Directories
SRC_DIR = ./src
INCLUDE_DIR = ./include
BIN_DIR = ./bin

# Compiler and flags
CC = gcc
CFLAGS = -I$(INCLUDE_DIR) -I$(RAYLIB_INCLUDE) -static
LDFLAGS = -L$(RAYLIB_DIR)/src -lraylib -lwinmm -lgdi32 -lopengl32 -static

# Implement both .c, .h and .cpp files
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
SRC_FILES += $(wildcard $(SRC_DIR)/*.cpp)
OUT = $(BIN_DIR)/exec

# Build the program
$(OUT): $(SRC_FILES)
	$(CC) $(SRC_FILES) -o $(OUT) $(CFLAGS) $(LDFLAGS)

# Clean the bin directory
clean:
	rm -f $(OUT)