CC = g++-14
CFLAGS = -Wall -Wextra

all: inter.out

inter.out: bf_interpreter.cpp
	$(CC) bf_interpreter.cpp -o inter.out -std=c++23 $(CFLAGS)

clean:
	rm inter.out
