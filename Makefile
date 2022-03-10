all: build

build: cardgame.c
	gcc cardgame.c -o test -g -Wall

run: 
	./test

val: test
	valgrind --leak-check=full ./test
	
clean:
	rm test in out