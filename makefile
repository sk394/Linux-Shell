# Makefile for simple linux shell

# Compiler
CC = gcc

# Source file
SF = myshell.c

# Output executable
TARGET = simple_shell

# Maximum number of processes
ULIMIT = 100

.PHONY: all clean run

# Default target
all: $(TARGET)

# Compile the shell
$(TARGET): $(SF)
	$(CC) -o $(TARGET) $(SF)

# Run the shell with ulimit
run: $(TARGET)
	@echo "Setting max process to $(ULIMIT).."
	@ulimit -u $(ULIMIT) && ./$(TARGET)

# Clean up
clean:
	rm -f $(TARGET)
