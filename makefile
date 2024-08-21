# Compiler
CC = gcc

# Executable name
TARGET = snek

# Source files
SRCS = main.c graphics.c game.c

# Object files
OBJS = $(SRCS:.c=.o)
INCLUDES = -I/usr/include/freetype2 -I/usr/include/libpng16

# Libraries
LIBS = -lm -lglfw -lGLEW -lGL -lfreetype

# Compiler flags
CFLAGS = $(INCLUDES) -Wall -Wextra -O2
LDFLAGS = $(LIBS)

# Default rule
all: $(TARGET)

# Rule to build the executable
$(TARGET): $(OBJS)
	$(CC) -o $@ $(OBJS) $(LIBS) $(LDFLAGS)

# Rule to compile source files into object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -f $(OBJS) $(TARGET)

# Rebuild the project
rebuild: clean all
