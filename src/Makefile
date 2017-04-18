CC=mpicxx
CFLAGS=-std=c++11 -Wall -g

TARGETS = reversi
OBJS = utils.o type.o board.o

all: $(TARGETS)

board.o: board.cpp
	$(CC) $(CFLAGS) -o $@ -c $^

type.o: type.cpp
	$(CC) $(CFLAGS) -o $@ -c $^

utils.o: utils.cpp
	$(CC) $(CFLAGS) -o $@ -c $^

reversi: reversi.cpp $(OBJS)
	$(CC) $(CFLAGS) -o $@ $+

.PHONY: all clean

clean:
	rm -f *o $(TARGETS)
