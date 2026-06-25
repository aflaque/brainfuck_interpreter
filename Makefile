CC = g++-14
CFLAGS = -Wall -pedantic

inter.out: bf_interpreter.cpp
	$(CC) bf_interpreter.cpp -o inter.out -std=c++23

clean:
	rm inter.out
