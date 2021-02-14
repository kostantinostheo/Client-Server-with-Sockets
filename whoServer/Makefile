CC=g++

CFLAGS=-c
CFLAGS2=-lpthread

all: whoServer

whoServer: whoServer.o whoServerMethods.o
	$(CC) -o whoServer whoServer.cpp whoServerMethods.cpp $(CFLAGS2)

whoServer.o: whoServer.cpp
	$(CC) $(CFLAGS) whoServer.cpp $(CFLAGS2)

whoServerMethods.o: whoServerMethods.cpp
	$(CC) $(CFLAGS) whoServerMethods.cpp $(CFLAGS2)

server:
	./whoServer -q 9004 -s 9003 -w 5 -b 5

clean:
	$(info Cleaning...)
	rm -rf *o whoServer
	clear
	

