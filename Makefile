# Define paths
RAYLIB_DIR = C:/raylib
RAYLIB_INCLUDE = $(RAYLIB_DIR)/src
RAYLIB_LIB = ./lib/libraylib.a

# Directories
SRC_DIR = ./src
BIN_DIR = ./bin

# Compiler and flags
CC = g++
CFLAGS = -I$(RAYLIB_INCLUDE) -static
LDFLAGS = -L$(RAYLIB_DIR)/src -lraylib -lwinmm -lgdi32 -lopengl32 -static 

# Implement both .c, .hpp and .cpp files
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
SRC_FILES += $(wildcard $(SRC_DIR)/*.h)
SRC_FILES += $(wildcard $(SRC_DIR)/*.hpp)
SRC_FILES += $(wildcard $(SRC_DIR)/*.cpp)
OUT = $(BIN_DIR)/exec

# Build the program
$(OUT): $(SRC_FILES)
	$(CC) $(SRC_FILES) -o $(OUT) $(CFLAGS) $(LDFLAGS)

# Clean the bin directory
clean:
	rm -f $(OUT)