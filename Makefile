all: bin/main

bin/main: obj/main.o obj/fib.o
	mkdir bin
	gcc -Wall -Werror obj/main.o obj/fib.o -o bin/main -lm

obj/main.o: include/fib.h src/main.c
	mkdir obj
	gcc -Wall -Werror -I include -c src/main.c -o obj/main.o -lm

obj/fib.o: include/fib.h src/fib.c
	gcc -Wall -Werror -I include -c src/fib.c -o obj/fib.o -lm

clean:
	rm -rf bin/ obj/
