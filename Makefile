CC = gcc # Flag for implicit rules
CFLAGS = -g -O0 -Wextra -Wall -Wshadow -Wformat=2

all: main.out

main.out: main.o
	${CC} -o $@ $^ -I.

main.o: main.c card.h
	${CC} ${CFLAGS} -c main.c

.PHONY: clean
clean:
	rm -f *.o *.out