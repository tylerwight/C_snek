# Compiler
CC = gcc

# Executable name
TARGET = snek

# Source files
SRCS = main.c graphics.c game.c

# Object files
OBJS = $(SRCS:.c=.o)
#INCLUDES = -I/usr/include/freetype2 -I/usr/include/libpng16 #linux
INCLUDES = -I/ucrt64/include/freetype2 -I/ucrt64/include/libpng16 -I/ucrt64/include/harfbuzz -I/ucrt64/include/glib-2.0 -I/ucrt64/lib/glib-2.0/include # windows
# Libraries
#LIBS = -lm -lglfw -lGLEW -lGL -lfreetype #linux
LIBS = -lm -lglfw3 -lglew32 -lopengl32 -lfreetype -lgdi32 -lpng -lz -lbrotlicommon -lbrotlidec -lbz2 -lharfbuzz -lgraphite2 -lrpcrt4 -lstdc++ -mwindows

# Compiler flags
CFLAGS = $(INCLUDES) -Wall -Wextra -O2
LDFLAGS = -L/ucrt64/lib -static $(LIBS)

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
