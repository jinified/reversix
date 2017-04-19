CC=mpicxx
CFLAGS=-std=c++11 -Wall -g -fopenmp

TARGETS = othellox
OBJS = utils.o type.o negascout.o evaluate.o

all: $(TARGETS)

negascout.o: negascout.cpp
	$(CC) $(CFLAGS) -o $@ -c $^

evaluate.o: evaluate.cpp
	$(CC) $(CFLAGS) -o $@ -c $^

type.o: type.cpp
	$(CC) $(CFLAGS) -o $@ -c $^

utils.o: utils.cpp
	$(CC) $(CFLAGS) -o $@ -c $^

othellox: othellox.cpp $(OBJS)
	$(CC) $(CFLAGS) -o $@ $+

.PHONY: all clean

clean:
	rm -f *o $(TARGETS)
