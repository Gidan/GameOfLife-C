# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -g -O0

BUILDDIR = build

# Output executable
TARGET = $(BUILDDIR)/game_of_life

# Source files
SRCS = ./src/main.c

# Object files
OBJS = $(SRCS:.c=.o)

# Include directories
INCLUDES = -I./include

# Linker flags
LDFLAGS = -lraylib -L./lib

# Default target
all: $(BUILDDIR) $(TARGET)

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

# Link the object files to create the executable
$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

# Compile source files to object files
%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Clean up the build artifacts
clean:
	rm -f $(OBJS) $(TARGET)

# Run the program
run: $(TARGET)
	./$(TARGET)

# Phony targets
.PHONY: all clean run
