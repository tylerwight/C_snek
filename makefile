# Compiler
CC = gcc

# Executable name
TARGET = snek

# Source files
SRCS = main.c graphics.c

# Object files
OBJS = $(SRCS:.c=.o)

# Libraries
LIBS = -lm -lglfw -lGLEW -lGL

# Compiler flags
CFLAGS = -Wall -Wextra -O2

# Default rule
all: $(TARGET)

# Rule to build the executable
$(TARGET): $(OBJS)
	$(CC) -o $@ $(OBJS) $(LIBS)

# Rule to compile source files into object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -f $(OBJS) $(TARGET)

# Rebuild the project
rebuild: clean all
