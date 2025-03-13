# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -g

# Source files
SRCS = parser.c lexer.c

# Object files
OBJS = $(SRCS:.c=.o)

# Executable name
TARGET = parser

# Default rule
all: $(TARGET)

# Linking step (Add -lm at the end)
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) -lm

# Compile C files into object files
%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -f $(OBJS) $(TARGET)

# Run the program
run: $(TARGET)
	./$(TARGET)
