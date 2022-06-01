CC=g++
CXXFLAGS=-std=c++11 -Wall -g
CFLAGS=-g -I. -lSDL2 -lGL -lGLEW -lSDL2_image -lSDL2_ttf
LDFLAGS=-lSDL2 -lGL -lGLEW -lSDL2_image -lSDL2_ttf
DEPS = Socket.h Chat.h Duck.h GameObject.h
OBJ = Socket.o Chat.o Duck.o GameObject.o
LIBS=-lpthread

%.o: %.cc $(DEPS)
	$(CC) -g -c -o $@ $< $(CFLAGS)

all: cs cc

cs: $(OBJ) ChatServer.o
	g++ -o $@ $^ $(CFLAGS) $(LIBS)

cc: $(OBJ) ChatClient.o
	g++ -o $@ $^ $(CFLAGS) $(LIBS)


.PHONY: clean

clean:
	rm -f *.o cs cc

