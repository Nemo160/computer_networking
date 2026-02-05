client: client.o tid.o
	gcc client.c -o client
	gcc tid.c -o tid

tid: tid.o
	gcc tid.c -o tid

client.o: client.c
	gcc -c client.c

tid.o: tid.c
	gcc -c tid.c


clean:
	rm -rf *.o 

target: dependencies
	action

