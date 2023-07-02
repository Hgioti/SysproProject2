objects = server.o queue.o

dataServer : $(objects)
	cc -o dataServer $(objects) -lpthread

server.o: server.c myLib.h
	gcc -w -c -g server.c

queue.o: queue.c
	gcc -c -g queue.c

clean:
	rm -rf *.o dataServer
