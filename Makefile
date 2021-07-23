CC = gcc # Flag for implicit rules
CFLAGS = -g

all: main.out

main.out: main.o list.o card.o
	${CC} -o $@ $^ -I.

main.o: main.c card.h
	${CC} ${CFLAGS} -c main.c

card.o: card.c card.h
	${CC} ${CFLAGS} -c card.c

list.o: list.c list.h
	${CC} ${CFLAGS} -c list.c

.PHONY: test
test: test_list.out
	./test_list.out

test_list.out: list.o test_list.o
	${CC} -o $@ $^

test_list.o: test_list.c list.h
	${CC} ${CFLAGS} -c test_list.c

.PHONY: clean
clean:
	rm -f *.o *.out