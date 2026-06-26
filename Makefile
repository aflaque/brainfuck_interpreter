CC = g++-14
CFLAGS = -Wall -Wextra

inter.out: bf_interpreter.cpp
	$(CC) bf_interpreter.cpp -o inter.out -std=c++23 $(CFLAGS)

clean:
	rm inter.out
