objects = client.o getFiles.o

remoteClient : $(objects)
	cc -o remoteClient $(objects)

client.o: client.c clientLib.h
	gcc -w -c -g client.c

getFiles.o: getFiles.c clientLib.h
	gcc -c -g getFiles.c

clean:
	rm -rf *.o remoteClient
