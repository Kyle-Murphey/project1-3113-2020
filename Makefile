all: project1

project1.o: project1.c
	gcc -c project1.c

project1: project1.o
	gcc project1.o -o project1

clean:
	rm -f *.o project1
