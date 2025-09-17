CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g

SOURCES = main.c list.c parser.c executor.c utils.c
OBJECTS = $(SOURCES:.c=.o)
TARGET = shell

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(TARGET) $(OBJECTS)

run: all
	./$(TARGET)