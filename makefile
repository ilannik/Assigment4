all: frequency.o

frequency.o: frequency.c
	gcc -Wall -g frequency.c -o frequency

clean:
	rm *.o *.out
