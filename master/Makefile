CC=g++

CFLAGS=-c

all: master

master: master.o executive.o pipes.o worker.o
	$(CC) -o master master.cpp executive.cpp pipes.cpp worker.cpp

master.o: master.cpp
	$(CC) $(CFLAGS) master.cpp

executive.o: executive.cpp
	$(CC) $(CFLAGS) executive.cpp

pipes.o: pipes.cpp
	$(CC) $(CFLAGS) pipes.cpp

worker.o: worker.cpp
	$(CC) $(CFLAGS) worker.cpp


exec_master: 
	./master -w 5 -b 5 -s 127.0.0.1 -p 9003 -i dir

clean:
	$(info Cleaning...)
	rm -rf *o master
	rm -rf log_file.*
	clear
	

