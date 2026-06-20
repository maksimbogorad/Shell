CC = gcc
CFLAGS = -g -Wall -Werror -pedantic -I./include
SOURCE = src/
INCLUDE = include/

OBJS = shell.o main.o executor.o ssignals.o list.o parser.o commands.o utils.o

TARGET = shell

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

shell.o: $(SOURCE)shell.c $(INCLUDE)shell.h
	$(CC) $(CFLAGS) -c $< -o $@

main.o: $(SOURCE)main.c $(INCLUDE)shell.h
	$(CC) $(CFLAGS) -c $< -o $@

executor.o: $(SOURCE)executor.c $(INCLUDE)executor.h
	$(CC) $(CFLAGS) -c $< -o $@

ssignals.o: $(SOURCE)ssignals.c $(INCLUDE)ssignals.h
	$(CC) $(CFLAGS) -c $< -o $@

list.o: $(SOURCE)list.c $(INCLUDE)list.h
	$(CC) $(CFLAGS) -c $< -o $@

commands.o: $(SOURCE)commands.c $(INCLUDE)commands.h
	$(CC) $(CFLAGS) -c $< -o $@

utils.o: $(SOURCE)utils.c $(INCLUDE)utils.h
	$(CC) $(CFLAGS) -c $< -o $@

parser.o: $(SOURCE)parser.c $(INCLUDE)parser.h
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: run clean