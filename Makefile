CC = gcc # Flag for implicit rules

.PHONY: test
test: test_list.out
	./test_list.out

test_list.out: list.o test_list.o
	${CC} -o $@ $^

list.o: list.h list.c
	${CC} -c list.c

test_list.o: list.h test_list.c
	${CC} -c test_list.c

.PHONY: clean
clean:
	rm -f *.o test_list main