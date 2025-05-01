CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c17 -Iinclude
OBJDIR = obj
SRCDIR = src
BINDIR = bin
TARGET = $(BINDIR)/ip_a_pretty

# List of source files
SOURCES = $(SRCDIR)/interface.c $(SRCDIR)/utils.c $(SRCDIR)/parser.c $(SRCDIR)/renderer.c $(SRCDIR)/main.c
OBJECTS = $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

# Create directories
.PHONY: directories
directories:
	mkdir -p $(OBJDIR)
	mkdir -p $(BINDIR)

# Default target
all: directories $(TARGET)

# Link object files to create the executable
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET)

# Compile source files into object files
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up generated files
clean:
	rm -rf $(OBJDIR) $(BINDIR)

.PHONY: all clean

run: all
	$(TARGET)
