# Compiler
CC = gcc

# Compiler flags
CFLAGS = -O0 -Wall -g3 -I ../ -I ../../include/ -I ../../src/FreeRTOS/Source/CMSIS_RTOS_V2 -I ../../src/FreeRTOS/Source/include -I ../../src/FreeRTOS/Source/portable/GCC/ARM_CM4F

# Source files
SRCS = ../../src/planet.c ../../src/spaceship.c commandsTest.c

# Object files
OBJS = $(SRCS:.c=.o)

# Executable name
TARGET = test

# Default target
all: $(TARGET)

# Compile source files into object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Link object files into executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

# Clean up object files and executable
clean:
	rm -f $(OBJS) $(TARGET)
	rm -f ../../src/$(OBJS)
	