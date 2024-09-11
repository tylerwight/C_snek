# Compiler
CC = gcc

# Executable name
TARGET = bin/snek

# Source and include directories
SRC_DIR = src
INC_DIR = include
BUILD_DIR = build

# Source files
SRCS = $(SRC_DIR)/main.c $(SRC_DIR)/graphics.c $(SRC_DIR)/game.c

# Object files (build directory + source file name with .o extension)
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

# Detect the operating system
ifeq ($(OS), Windows_NT)
    # Windows-specific settings
    INCLUDES = -I$(INC_DIR) -I/ucrt64/include/freetype2 -I/ucrt64/include/libpng16 -I/ucrt64/include/harfbuzz -I/ucrt64/include/glib-2.0 -I/ucrt64/lib/glib-2.0/include
    LIBS = -lm -lglfw3 -lglew32 -lopengl32 -lfreetype -lpng -lz -lbrotlidec -lbrotlienc -lbrotlicommon -lgdi32 -lbz2 -lharfbuzz -lgraphite2 -lrpcrt4 -lstdc++
    LDFLAGS = -L/ucrt64/lib -static $(LIBS) -mwindows
else
    # Linux-specific settings
    INCLUDES = -I$(INC_DIR) -I/usr/include/freetype2 -I/usr/include/libpng16
    LIBS = -lm -lglfw -lGLEW -lGL -lfreetype -lopenal -lalut
    LDFLAGS = $(LIBS)
endif

# Compiler flags
CFLAGS = $(INCLUDES) -Wall -Wextra -O2

# Default rule
all: $(TARGET)

# Rule to build the executable
$(TARGET): $(OBJS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)

# Rule to compile source files into object files (with new paths)
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Ensure the build directory exists
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Clean up build files
clean:
	rm -f $(BUILD_DIR)/*.o $(TARGET)

# Rebuild the project
rebuild: clean all