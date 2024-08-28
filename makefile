# Compiler
CC = gcc

# Executable name
TARGET = snek

# Source files
SRCS = main.c graphics.c game.c

# Object files
OBJS = $(SRCS:.c=.o)

# Detect the operating system
ifeq ($(OS), Windows_NT)
    # Windows-specific settings
    INCLUDES = -I/ucrt64/include/freetype2 -I/ucrt64/include/libpng16 -I/ucrt64/include/harfbuzz -I/ucrt64/include/glib-2.0 -I/ucrt64/lib/glib-2.0/include
    LIBS = -lm -lglfw3 -lglew32 -lopengl32 -lfreetype -lpng -lz -lbrotlicommon -lbrotlidec -lgdi32 -lbz2 -lharfbuzz -lgraphite2 -lrpcrt4 -lstdc++
    LDFLAGS = -L/ucrt64/lib -static $(LIBS) -mwindows
else
    # Linux-specific settings
    INCLUDES = -I/usr/include/freetype2 -I/usr/include/libpng16
    LIBS = -lm -lglfw -lGLEW -lGL -lfreetype 
    LDFLAGS = $(LIBS)
endif

# Compiler flags
CFLAGS = $(INCLUDES) -Wall -Wextra -O2

# Default rule
all: $(TARGET)

# Rule to build the executable
$(TARGET): $(OBJS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)

# Rule to compile source files into object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -f $(OBJS) $(TARGET)

# Rebuild the project
rebuild: clean all