CC=g++

CFLAGS=-c 
CFLAGS2=-lpthread

all: whoClient

whoClient: whoClient.o whoClientMethods.o
	$(CC) -o whoClient whoClient.cpp whoClientMethods.cpp $(CFLAGS2)

whoClient.o: whoClient.cpp
	$(CC) $(CFLAGS) whoClient.cpp $(CFLAGS2)

whoClientMethods.o: whoClientMethods.cpp
	$(CC) $(CFLAGS) whoClientMethods.cpp $(CFLAGS2)

client: 
	./whoClient -q queryFile -w 5 -sp 9004 -sip 127.0.0.1 

clean:
	$(info Cleaning...)
	rm -rf *o whoClient
	clear
	

