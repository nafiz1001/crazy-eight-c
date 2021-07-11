CC = gcc # Flag for implicit rules
DEBUG_FLAGS = -g

main.out: main.o list.o
	${CC} -o $@ $^

main.o: main.c
	${CC} ${DEBUG_FLAGS} -c main.c

list.o: list.h list.c
	${CC} ${DEBUG_FLAGS} -c list.c

.PHONY: test
test: test_list.out
	./test_list.out

test_list.out: list.o test_list.o
	${CC} -o $@ $^

test_list.o: list.h test_list.c
	${CC} ${DEBUG_FLAGS} -c test_list.c

.PHONY: clean
clean:
	rm -f *.o *.out